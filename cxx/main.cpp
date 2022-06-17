#include <map>
#include <array>
#include <iostream>
#include <gpiod.hpp>

#define RASPI_GPIO_CHIP "gpiochip0"


std::array<std::array<bool, 5>, 5> get_layout()
{
    std::array<std::array<bool, 5>, 5> layouts{false};

    // init all to false
    for (auto &layer: layouts)
    {
        for (auto &led: layer)
        {
            led = false;
        }
    }

    // load from file
}

int main()
{
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
}
