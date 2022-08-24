// System Libraries
#include <array>
#include <chrono>
#include <thread>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <exception>
#include <filesystem>
// Dependencies
#include <gpiod.hpp>
#include <nlohmann/json.hpp>
// Files
#include "Button.h"


using values_t = std::array<bool, 5>;
using lines_t = std::array<values_t, 5>;
using layers_t = std::array<lines_t, 5>;

class Main
{
private:
#pragma region lines
    /// reference to the lines chip
    gpiod::chip chip;

    /// pins for each layer toggle
    std::array<gpiod::line, 5> layers;

#pragma region led
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
#pragma endregion
#pragma endregion

public:
    Main()
    {
        // init chip
        chip = gpiod::chip("gpiochip0", gpiod::chip::OPEN_BY_NAME);

#pragma region aquire lines
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
            layer.request({layer.name(), gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);
        }
        std::cout << "All Layers acquired" << std::endl;
#pragma endregion

#pragma region led
        // reset pin setup (pull down)
        pin_reset = chip.get_line(18);
        pin_reset.request({pin_reset.name(), gpiod::line_request::DIRECTION_OUTPUT, 0}, 1);
        std::cout << "Reset pin acquired" << std::endl;

        // shift pin setup (pull down)
        pin_shift = chip.get_line(14);
        pin_shift.request({pin_shift.name(), gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);
        std::cout << "Shift pin acquired" << std::endl;

        // store pin setup (pull down)
        pin_store = chip.get_line(15);
        pin_store.request({pin_store.name(), gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);
        std::cout << "Store pin acquired" << std::endl;

        // datain pin setup (pull down)
        pin_datain = chip.get_line(12);
        pin_datain.request({pin_datain.name(), gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);
        std::cout << "Datain pin acquired" << std::endl;

        // special pin setup (pull down)
        pin_special = chip.get_line(13);
        pin_special.request({pin_special.name(), gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);
        std::cout << "Special pin acquired" << std::endl;
#pragma endregion
#pragma endregion
    }

private:
#pragma region line controll abstraction methods
    void reset()
    {
        pin_reset.set_value(0);
        pin_reset.set_value(1);
        pin_special.set_value(0);
    }

    void shift()
    {
        pin_shift.set_value(1);
        pin_shift.set_value(0);
    }

    void store()
    {
        pin_store.set_value(1);
        pin_store.set_value(0);
    }
#pragma endregion

public:
    void loop()
    {
        layers[0].set_value(1);
        layers[1].set_value(1);

        pin_special.set_value(1);
        for (int i = 0; i < 25; ++i) {
            if (i == 5 or i == 9) {
                pin_shift.set_value(1);
            }else {
                pin_shift.set_value(0);
            }
            shift();
        }

        std::cin.ignore();
    }
};

int main()
{
    try
    {
        Main m;
        while (true)
        {
            m.loop();
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "failure: " << e.what() << std::endl;
    }
}
