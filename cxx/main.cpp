#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <exception>

#include <gpiod.hpp>
#include <nlohmann/json.hpp>


#define RASPI_GPIO_CHIP "gpiochip0"


struct Frame
{
    /// the time in milliseconds, how long the frame should be visible
    unsigned int frame_time;
    /// for each layer a stream of booleans, encoding which led is on
    std::array<std::array<std::array<bool, 5>, 5>, 5> data;
};

class Main
{
private:
    /// a list of frames, each representing a current state of the cube
    std::vector<Frame> frames;

    /// pins for each layer toggle
    std::array<gpiod::line, 5> layers;

#pragma region lines
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

    static std::vector<Frame> parse_layout()
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

            // layer data
            for (int i = 0; i < 5; i++)
            {
                for (int j = 0; j < 5; j++)
                {
                    for (int k = 0; k < 5; k++)
                    {
                        const auto &value = _frame["layers"][i][j][k];
                        frame.data[i][j][k] = value;
                    }
                }
            }

            // reverse each lain and each
            for (auto &layer: frame.data)
            {
                for (auto &lain: layer)
                {
                    std::reverse(std::begin(lain), std::end(lain));
                }
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

#pragma region bluetooth
    void save()
    {

    }

    void load()
    {

    }

    void next()
    {

    }

    void previous()
    {

    }
#pragma endregion

#pragma region bluetooth
    void pair()
    {

    }

    void sync_files()
    {

    }
#pragma endregion

public:
    Main()
    {
        // parse input data
        frames = parse_layout();

        // init chip
        gpiod::chip chip("gpiochip0", gpiod::chip::OPEN_BY_NAME);

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
            // todo name maybe has to be set explicitly
            layer.request({layer.name(), gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);
        }
        std::cout << "All Layers acquired" << std::endl;
#pragma endregion

#pragma region led
        // reset pin setup
        pin_reset = chip.get_line(18);
        pin_reset.request({"GPIO12", gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);
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

        // set pin reset initially to off
        pin_reset.set_value(1);
#pragma endregion

#pragma region I/O
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
        }
        if (is_falling_edge(line_next, _next_edge))
        {
            // todo
            //  activate next setting
            std::cout << "next setting button press" << std::endl;
        }
        if (is_falling_edge(line_previous, _previous_edge))
        {
            // todo
            //  activate next setting
            std::cout << "previous setting button press" << std::endl;
        }
        if (is_falling_edge(line_power, _power_edge))
        {
            // todo
            //  switch on/ off cube
            std::cout << "switch on/ off button press" << std::endl;
        }
    }

    void loop()
    {
        for (auto &frame: frames)
        {
            // timer is used to determine if we should switch to next frame
            const auto starting_time = std::chrono::steady_clock::now();
            // convert the frame time to milliseconds
            const auto max_frame_time = std::chrono::milliseconds(frame.frame_time);

            // reset all leds for next frame
            reset();

            // replay current frame as fast as possible, as often as possible
            // and only at the end of the current frame duration continue with next frame
            while (true)
            {
                // poll for possible button events
                poll();

                int i_layer = 0;
                for (const auto &layer_data: frame.data)
                {
                    // disable all previous layer, to ensure that only one layer is turned on
                    for (auto &layer_pin: layers)
                    {
                        layer_pin.set_value(0);
                    }
                    layers[i_layer].set_value(1); // enable current layer

                    int i_line = 0;
                    for (const auto &line_data: layer_data)
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

                    store(); // store each layer

                    // break loop, only if the elapsed time is larger than the max frame time
                    auto current_time = std::chrono::steady_clock::now();
                    auto elapsed_time =
                        std::chrono::duration_cast<std::chrono::milliseconds>(current_time - starting_time);
                    if (elapsed_time >= max_frame_time)
                    {
                        goto break_while;
                    }

                    ++i_layer;
                }
            }
            break_while:;
        }
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
