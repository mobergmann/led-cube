#include <array>
#include <chrono>
#include <thread>
#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <exception>
#include <filesystem>

#include <gpiod.hpp>
#include <nlohmann/json.hpp>


using values_t = std::array<bool, 5>;
using lines_t = std::array<values_t, 5>;
using layers_t = std::array<lines_t, 5>;

namespace fs = std::filesystem;

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
#pragma region file management
    /// a list of all files in the data folder
    std::vector<std::string> files;
    std::string current_file;

#pragma endregion

    /// a list of frames, each representing a current state of the cube
    std::vector<Frame> frames;

    /// boolean indicating if the led cube should be on
    bool cube_on = true;

#pragma region lines
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

#pragma region I/O
    /// led for showing if the cube is in pairing mode
    gpiod::line line_pairing_led;

    /// button for enabling pairing mode (pull down)
    gpiod::line line_bluetooth;
    bool _bluetooth_edge = true;

    /// button for iterating to next led setting (pull down)
    gpiod::line line_next;
    bool _next_edge = true;

    /// button for iterating to previous led setting (pull up)
    gpiod::line line_previous;
    bool _previous_edge = true;

    /// button for enabling/ disabling the cube (pull up)
    gpiod::line line_power;
    bool _power_edge = true;
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

    static void bluetooth_deamon(Main* m)
    {
        m->update_file_list();

        for (int i = 0; i < 5; ++i)
        {
            // blink bluetooth pairing led
            m->line_pairing_led.set_value(1);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            m->line_pairing_led.set_value(0);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }

    std::vector<Frame> parse_layout()
    {
        std::vector<Frame> frames;

        // load from file
        std::ifstream stream(current_file);

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

    void update_file_list()
    {
        std::string tmp = current_file;

        files.clear();

        // search all new files
        std::string data_dir_path = fs::path(getenv("HOME")) / ".led-cube" / "custom";
        std::string default_dir_path = fs::path(getenv("HOME")) / ".led-cube" / "default";
        // add default cube configurations
        for (const auto &file: fs::recursive_directory_iterator(default_dir_path))
        {
            std::cout << "Found file: " << file.path() << std::endl;
            files.push_back(file.path());
        }
        // add custom cube configurations
        for (const auto &file: fs::recursive_directory_iterator(data_dir_path))
        {
            std::cout << "Found file: " << file.path() << std::endl;
            files.push_back(file.path());
        }

        // search element in list, if not exits use fallback option
        current_file = *files.begin(); // fallback option
        for (const auto &val: files)
        {
            if (val == tmp)
            {
                current_file = val;
                break;
            }
        }
        std::cout << "New Configuration: " << current_file << std::endl;
    }

    void next()
    {
        // search element in list, if not exits use fallback option
        std::string tmp = current_file;
        current_file = *files.begin(); // fallback option
        for (int i = 0; i < files.size(); ++i)
        {
            const auto &it = files[i];

            if (it == tmp)
            {
                // when at end, the next element is the beginning of the list
                if (it == *files.end())
                {
                    current_file = *files.begin();
                    break;
                }
                    // when found use previous element as new one
                else
                {
                    current_file = files[i+1];
                    break;
                }
            }
        }
        std::cout << "New Configuration: " << current_file << std::endl;
    }

    void previous()
    {
        // search element in list, if not exits use fallback option
        std::string tmp = current_file;
        current_file = *files.begin(); // fallback option
        for (int i = 0; i < files.size(); ++i)
        {
            const auto &it = files[i];

            if (it == tmp)
            {
                // when at beginning, the previous element is the end of the list
                if (it == *files.begin())
                {
                    current_file = *files.end();
                    break;
                }
                    // when found use previous element as new one
                else
                {
                    current_file = files[i-1];
                    break;
                }
            }
        }
        std::cout << "New Configuration: " << current_file << std::endl;
    }

    /**
     * Polls for button events and then process these events
     */
    void poll()
    {
        // Bluetooth button
        if (is_rising_edge(line_bluetooth, _bluetooth_edge))
        {
            std::cout << "bluetooth button pressed" << std::endl;
            new std::thread(&bluetooth_deamon, this);
        }

        // Previous Button
        if (is_rising_edge(line_previous, _previous_edge))
        {
            std::cout << "previous setting button pressed" << std::endl;
            previous();
        }

        // Next Button
        if (is_rising_edge(line_next, _next_edge))
        {
            std::cout << "next setting button pressed" << std::endl;
            next();
        }

        // Power Button
        if (is_rising_edge(line_power, _power_edge))
        {
            std::cout << "switch on/ off button press" << std::endl;
            cube_on = not cube_on;
        }
    }

    void set_leds(const layers_t &frame_data)
    {
        if (not cube_on)
        {
            return;
        }

        int i_layer = 0;
        for (const auto &layer_data: frame_data)
        {
            // reset all leds for next frame
            reset();

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


            ++i_layer;
        }
    }

#pragma region
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
    Main()
    {
        // get all files
        update_file_list();

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
        // Pairing Mode LED (pull down)
        line_pairing_led = chip.get_line(11);
        line_pairing_led.request({"GPIO11", gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);
        std::cout << "Pairing Mode LED acquired" << std::endl;

        // bluetooth pairing button (pull up)
        line_bluetooth = chip.get_line(6);
        line_bluetooth.request({"GPIO6", gpiod::line_request::DIRECTION_INPUT, 0}, 1);
        std::cout << "bluetooth pairing pin acquired" << std::endl;

        // previous setting button (pull up)
        line_previous = chip.get_line(5);
        line_previous.request({"GPIO5", gpiod::line_request::DIRECTION_INPUT, 0}, 1);
        std::cout << "Previous setting pin acquired" << std::endl;

        // next setting button (pull down)
        line_next = chip.get_line(4);
        line_next.request({"GPIO4", gpiod::line_request::DIRECTION_INPUT, 0}, 1);
        std::cout << "Next setting pin acquired" << std::endl;

        // power on/ off button (pull down)
        line_power = chip.get_line(7);
        line_power.request({"GPIO7", gpiod::line_request::DIRECTION_INPUT, 0}, 1);
        std::cout << "power on/ off pin acquired" << std::endl;
#pragma endregion
#pragma endregion
    }

    void loop()
    {
        for (auto &frame: frames)
        {
            // timer is used to determine if we should switch to next frame
            const auto starting_time = std::chrono::steady_clock::now();
            // convert the frame time to milliseconds
            const auto max_frame_time = std::chrono::milliseconds(frame.frame_time);

            // replay current frame as fast as possible, as often as possible
            // and only at the end of the current frame duration continue with next frame
            while (true)
            {
                // poll for possible button events
                poll();

                set_leds(frame.data);

                // break loop, only if the elapsed time is larger than the max frame time
                auto current_time = std::chrono::steady_clock::now();
                auto elapsed_time =
                    std::chrono::duration_cast<std::chrono::milliseconds>(current_time - starting_time);
                if (elapsed_time >= max_frame_time)
                {
                    goto break_while;
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
