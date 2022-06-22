#include <map>
#include <array>
#include <iostream>
#include <fstream>
#include <typeinfo>

#include <gpiod.hpp>
#include <nlohmann/json.hpp>


#define RASPI_GPIO_CHIP "gpiochip0"


struct Frame {
    double frame_time;
    std::array<std::array<std::array<bool, 5>, 5>, 5> data;
};

std::vector<Frame> get_layout() {
    std::vector<Frame> frames;

    // load from file
    std::ifstream stream("data.json"); // todo proper/ dynamic file loading

    nlohmann::json file;
    stream >> file;
    stream.close();

    // parse json
    auto _frames = file["frames"];
    for (auto &_frame: _frames) {
        Frame frame{};

        // frame time
        frame.frame_time = _frame["frame-time"];

        // layer data
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                for (int k = 0; k < 5; k++) {
                    const auto &value = _frame["layers"][i][j][k];
                    frame.data[i][j][k] = value;
                }
            }
        }

        frames.push_back(frame);
    }

    return frames;
}

void reset() {
    throw std::runtime_error("Not Implemented");
}

void shift() {
    throw std::runtime_error("Not Implemented");
}

void store() {
    throw std::runtime_error("Not Implemented");
}

int main() {
#pragma region init
    // parse input data
    auto frames = get_layout();

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
    for (auto &layer: layers) {
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

    // store pin setup
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

    // todo implement delay check, for proper timing
    while (true) {
        for (auto &frame: frames) {
            // reset all leds for next frame
            reset();

            // each layer
            for (int _ = 0; _ < frame.data.size(); ++_) {
                auto x = frame.data[_];

                // turn all layers off
                for (auto &layer: layers) {
                    layer.set_value(0);
                }
                // activate current layer
                auto layer = layers[_];
                layer.set_value(1);

                // shift all values into the leds/ registerst
                for (const auto &__layer: frame.data) {
                    for (const auto &__line: __layer) {
                        for (const auto &__value: __line) {
                            if (__value) {
                                pin_datain.set_value(1);
                            } else {
                                pin_datain.set_value(0);
                            }
                            // pin_datain.set_value(__value);
                            shift();
                        }
                    }
                }

                // store data, to display result
                store();
            }
        }
    }
