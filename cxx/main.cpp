#include <map>
#include <array>
#include <fstream>
#include <iostream>

#include <gpiod.hpp>
#include <nlohmann/json.hpp>


/// pins for each layer toggle
std::array<gpiod::line, 5> layers;

/// reset pin
gpiod::line pin_reset;

/// shift clock pin
gpiod::line pin_shift;

/// store clock pin
gpiod::line pin_store;

/// datain pin
gpiod::line pin_datain;

/// special pin, which cannot be accessed by shifting
gpiod::line pin_special;

struct Frame
{
    /// the time in seconds, how long the frame should be visible
    double frame_time;
    /// for each layer a stream of bools, encoding which led is on
    std::array<std::array<bool, 25>, 5> data;
};

std::vector<Frame> get_layout()
{
    std::vector<Frame> frames;

    // load from file
    std::ifstream stream("data.json"); // todo proper/ dynamic file loading

    nlohmann::json file;
    stream >> file;
    stream.close();

    // parse json
    auto _frames = file["frames"];
    for (auto &_frame: _frames)
    {
        Frame frame{};

        // frame time
        frame.frame_time = _frame["frame-time"];

        // frame data
        auto iter = std::string(_frame["data"]);
        for (int i = 0; i < iter.length(); ++i)
        {
            auto c = iter[i];

            // 0 for false and any other char for true
            int column = i / 5;
            int row = i % 5;
            frame.data[column][row] = c != '0';
        }

        frames.push_back(frame);
    }

    return frames;
}

void reset()
{
    pin_reset.set_value(0);
    pin_reset.set_value(1);
}

void shift()
{
    pin_shift.set_value(0);
    pin_shift.set_value(1);
}

void store()
{
    pin_store.set_value(0);
    pin_store.set_value(1);
}

int main()
{
#pragma region init
    // parse input data
    auto frames = get_layout();

    // init chip
    gpiod::chip chip("gpiochip0", gpiod::chip::OPEN_BY_NAME);

#pragma region layers
    // save layers to array
    layers = {
        chip.get_line(20),
        chip.get_line(21),
        chip.get_line(23),
        chip.get_line(24),
        chip.get_line(25)
    };

    // initialize layers
    for (auto &layer: layers)
    {
        // todo name maybe has to be set explicitly
        layer.request({layer.name(), gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);
    }
    std::cout << "All Layers acquired" << std::endl;
#pragma endregion

    // reset pin setup
    pin_reset = chip.get_line(12);
    pin_reset.request({"GPIO12", gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);
    std::cout << "Reset pin acquired" << std::endl;

    // shift pin setup
    pin_shift = chip.get_line(14);
    pin_shift.request({"GPIO14", gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);
    std::cout << "Reset pin acquired" << std::endl;

    // store pin setup
    pin_store = chip.get_line(15);
    pin_store.request({"GPIO15", gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);
    std::cout << "Reset pin acquired" << std::endl;

    // datain pin setup
    pin_datain = chip.get_line(12);
    pin_datain.request({"GPIO12", gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);
    std::cout << "Datain pin acquired" << std::endl;

    // special pin setup
    pin_special = chip.get_line(13);
    pin_special.request({"GPIO13", gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);
    std::cout << "Special pin acquired" << std::endl;
#pragma endregion

    pin_reset.set_value(1);

    // todo implement delay check, for proper timing
    while (true)
    {
        for (auto &frame: frames)
        {
            // reset all leds for next frame
            reset();

            for (int i = 0; i < frame.data.size(); ++i)
            {
                auto layer_data = frame.data[i];

                // disable all previous layer, to ensure that only one layer is turned on
                for (auto &layer: layers)
                {
                    layer.set_value(0);
                }
                layers[i].set_value(1); // enable current layer

                // set each led pin
                for (int j = 0; j < layer_data.size(); ++j)
                {
                    bool led_value = layer_data[j];

                    // turn on special pin if end of shift register reached (layer 5 and pin 25)
                    if (i == 5 && j == 24)
                    {
                        pin_special.set_value(led_value);
                    }
                    else
                    {
                        pin_datain.set_value(led_value);
                        shift(); // only shift, when not the last pin
                    }
                }

                store(); // store each layer
            }
        }
    }
}
