#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <exception>

#include <gpiod.hpp>


class Main
{
private:
#pragma region I/O
    /// led for showing if the cube is in pairing mode
    gpiod::line line_pairing_led;

    /// button for enabling pairing mode
    gpiod::line line_bluetooth;
    bool _bluetooth_edge = false;

    /// button for iterating to next led setting
    gpiod::line line_next;
    bool _next_edge = false;

    /// button for iterating to previous led setting
    gpiod::line line_previous;
    bool _previous_edge = false;

    /// button for enabling/ disabling the cube
    gpiod::line line_power;
    bool _power_edge = false;
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
        throw std::runtime_error("Not Implemented");
    }

public:
    Main()
    {
        // init chip
        gpiod::chip chip("gpiochip0", gpiod::chip::OPEN_BY_NAME);

#pragma region aquire lines
        // Pairing Mode LED
        line_pairing_led = chip.get_line(16);
        line_pairing_led.request({"GPIO", gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);
        std::cout << "Pairing Mode LED acquired" << std::endl;

        // bluetooth pairing button
        line_bluetooth = chip.get_line(26);
        line_bluetooth.request({"GPIO", gpiod::line_request::DIRECTION_INPUT, 0}, 0);
        std::cout << "bluetooth pairing pin acquired" << std::endl;

        // next setting button
        line_next = chip.get_line(19);
        line_next.request({"GPIO", gpiod::line_request::DIRECTION_INPUT, 0}, 0);
        std::cout << "Next setting pin acquired" << std::endl;

        // previous setting button
        line_previous = chip.get_line(6);
        line_previous.request({"GPIO", gpiod::line_request::DIRECTION_INPUT, 0}, 0);
        std::cout << "Previous setting pin acquired" << std::endl;

        // power on/ off button
        line_power = chip.get_line(5);
        line_power.request({"GPIO", gpiod::line_request::DIRECTION_INPUT, 0}, 0);
        std::cout << "power on/ off pin acquired" << std::endl;
#pragma endregion
    }

    /**
     * Polls for button events and then process these events
     */
    void poll()
    {
        // check for button press
        if (is_falling_edge(line_bluetooth, _bluetooth_edge))
        {
            // todo
            //  activate bluetooth protocol
            //  blink bluetooth led
            std::cout << "bluetooth button press" << std::endl;
            line_pairing_led.set_value(1);
        }
        else
        {
            line_pairing_led.set_value(0);
        }
        if (is_falling_edge(line_next, _next_edge))
        {
            // todo
            //  activate next setting
            std::cout << "next setting button press" << std::endl;
        }
        if (is_falling_edge(line_previous, _previous_edge)) // todo this buttons doesnt work
        {
            // todo
            //  activate next setting
            std::cout << "previous setting button press" << std::endl;
        }
        if (is_falling_edge(line_power, _power_edge)) // todo this buttons doesnt work
        {
            // todo
            //  switch on/ off cube
            std::cout << "switch on/ off button press" << std::endl;
        }
    }

    void loop()
    {
        // poll for possible button events
        poll();
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
