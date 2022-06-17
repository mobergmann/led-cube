#include <map>
#include <array>
#include <iostream>
#include <fstream>

#include <gpiod.hpp>
#include <nlohmann/json.hpp>


#define RASPI_GPIO_CHIP "gpiochip0"


struct Frame {
    double frame_time;
    std::array<std::array<bool, 5>, 5> data{};
};

std::vector<Frame> get_layout()
{
    std::vector<Frame> frames;

    // load from file
    std::ifstream stream("data.json");

    nlohmann::json file;
    stream >> file;
    stream.close();

    // parse json
    auto _frames = file["frames"];
    for (auto &_frame: _frames) {
        Frame frame;
        frame.frame_time = _frame["frame-time"];

        frame.data = std::array<std::array<bool, 5>, 5>();

        auto iter = std::string(_frame["data"]);
        for (int i = 0; i < iter.length(); ++i) {
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

int main()
{
    get_layout();
//    return 0;

#pragma region init

    // init chip
    gpiod::chip chip(RASPI_GPIO_CHIP, gpiod::chip::OPEN_BY_NAME);

#pragma region layers
    // save layers to array
    std::array<gpiod::line, 5> layers{
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
    gpiod::line pin_reset;
    pin_reset = chip.get_line(12);
    pin_reset.request({"GPIO12", gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);
    std::cout << "Reset pin acquired" << std::endl;

    // shift pin setup
    gpiod::line pin_shift;
    pin_shift = chip.get_line(14);
    pin_shift.request({"GPIO14", gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);
    std::cout << "Reset pin acquired" << std::endl;

    // reset pin setup
    gpiod::line pin_store;
    pin_store = chip.get_line(15);
    pin_store.request({"GPIO15", gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);
    std::cout << "Reset pin acquired" << std::endl;

    // datain pin setup
    gpiod::line pin_datain;
    pin_datain = chip.get_line(12);
    pin_datain.request({"GPIO12", gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);
    std::cout << "Datain pin acquired" << std::endl;

    // special pin setup
    gpiod::line pin_special;
    pin_special = chip.get_line(13);
    pin_special.request({"GPIO13", gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);
    std::cout << "Special pin acquired" << std::endl;
#pragma endregion
}
