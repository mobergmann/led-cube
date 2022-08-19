#include <array>
#include <chrono>
#include <thread>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <exception>

#include <gpiod.hpp>
#include <nlohmann/json.hpp>


using values_t = std::array<bool, 5>;
using lines_t = std::array<values_t, 5>;
using layers_t = std::array<lines_t, 5>;


struct Frame
{
    /// the time in milliseconds, how long the frame should be visible
    unsigned int frame_time;
    /// for each layer a stream of booleans, encoding which led is on
    layers_t data;
};

class Main
{
private:
    lines_t line_data;

#pragma region lines
    /// pins for each layer toggle
    gpiod::line layer;

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

    gpiod::line line_reset_btn;
    bool _line_reset_btn_edge = true;
#pragma endregion

private:
    static bool is_falling_edge(const gpiod::line& line, bool& edge)
    {
        if (line.get_value() != edge)
        {
            if (edge)
            {
                edge = false;
                return true;
            }
        }
        else
        {
            return false;
        }
        edge = true;
        return false;
    }

    static bool is_rising_edge(const gpiod::line& line, bool& edge)
    {
        if (line.get_value() != edge)
        {
            if (not edge)
            {
                edge = true;
                return true;
            }
        }
        else
        {
            return false;
        }
        edge = false;
        return false;
    }

    static lines_t gen_data()
    {
        lines_t data;

        // layer data
        for (int j = 0; j < 5; j++)
        {
            for (int k = 0; k < 5; k++)
            {
                data[j][k] = true;
            }
        }

        return data;
    }

    void reset()
    {
        pin_reset.set_value(1);
        pin_reset.set_value(0);
        pin_special.set_value(0);
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

public:
    Main()
    {
        // parse input data
        line_data = gen_data();

        // init chip
        gpiod::chip chip("gpiochip0", gpiod::chip::OPEN_BY_NAME);

#pragma region aquire lines
#pragma region layers
        // save layers to array
        layer = chip.get_line(20);
        layer.request({layer.name(), gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);
        std::cout << "All Layers acquired" << std::endl;
#pragma endregion

#pragma region led
        // reset pin setup
        pin_reset = chip.get_line(18);
        pin_reset.request({"GPIO18", gpiod::line_request::DIRECTION_OUTPUT, 0}, 1);
        std::cout << "Reset pin acquired" << std::endl;

        // shift pin setup
        pin_shift = chip.get_line(14);
        pin_shift.request({"GPIO14", gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);
        std::cout << "Shift pin acquired" << std::endl;

        // store pin setup
        pin_store = chip.get_line(15);
        pin_store.request({"GPIO15", gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);
        std::cout << "Store pin acquired" << std::endl;

        // datain pin setup
        pin_datain = chip.get_line(12);
        pin_datain.request({"GPIO12", gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);
        std::cout << "Datain pin acquired" << std::endl;

        // special pin setup
        pin_special = chip.get_line(13);
        pin_special.request({"GPIO13", gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);
        std::cout << "Special pin acquired" << std::endl;
#pragma endregion

#pragma region I/O
        // bluetooth pairing button (pull up)
        line_reset_btn = chip.get_line(6);
        line_reset_btn.request({"GPIO6", gpiod::line_request::DIRECTION_INPUT, 0}, 1);
        std::cout << "line_reset_btn pin acquired" << std::endl;
#pragma endregion
#pragma endregion
    }

    /**
     * Polls for button events and then process these events
     */
    void poll()
    {
        // PULL UP Bluetooth button
        if (is_rising_edge(line_reset_btn, _line_reset_btn_edge))
        {
            std::cout << "Reset Button Pressed" << std::endl;

            reset();
            store();
        }
    }

    void loop()
    {
        layer.set_value(1);

        int i_line = 0;
        for (const auto &line_data: line_data)
        {
            int i_value = 0;
            for (const auto &led_value: line_data)
            {
                // turn on special pin if end of shift register reached (layer 5 and pin 25)
                if (i_line == 4 && i_value == 4)
                {
                    pin_special.set_value(led_value);
                }
                else
                {
                    pin_datain.set_value(led_value);
                    shift(); // only shift, when not the last pin
                }

                ++i_value;
            }

            ++i_line;
        }

        store();

        while (true)
        {
            poll();
        }
    }
};

int main()
{
    try
    {
        Main m;
        m.loop();
    }
    catch(const std::exception& e)
    {
        std::cerr << "failure: " << e.what() << std::endl;
    }
}
