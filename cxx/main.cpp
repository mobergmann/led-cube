// System Libraries
#include <array>
#include <chrono>
#include <thread>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <algorithm>
#include <exception>
#include <filesystem>
// Dependencies
#include <gpiod.hpp>
#include <nlohmann/json.hpp>
// Files
#include "include/Button.h"
#include "include/Frame.h"
#include "include/FileTransfer.h"


namespace fs = std::filesystem;


class Main
{
private:
    /// a list of frames, each representing a current state of the cube
    std::vector<Frame> frames;

    /// boolean indicating if the led cube should be on
    bool cube_on;

#pragma region file management
    /// a list of all files in the data folder
    std::vector<std::string> files;

    /// stores the current configuration file
    std::string current_file;
#pragma endregion

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
    /// the value for the special pin, which needs to be applied in the store method
    bool pin_special_val;
#pragma endregion

#pragma region I/O
    /// led for showing if the cube is in pairing mode
    gpiod::line line_blink_led;

    /// button for pulling new data from usb
    Button *line_usb;

    /// button for iterating to next led setting
    Button *line_next;

    /// button for iterating to previous led setting
    Button *line_previous;

    /// button for enabling/ disabling the cube
    Button *line_power;
#pragma endregion
#pragma endregion

public:
    Main() : cube_on(true), pin_special_val(false)
    {
        // create all folders
        init_fs();

        // get all files
        update_file_list();

        // parse input data
        parse_layout();

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

#pragma region I/O
        // Pairing Mode LED (pull down)
        line_blink_led = chip.get_line(11);
        line_blink_led.request({line_blink_led.name(), gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);
        std::cout << "Pairing Mode LED acquired" << std::endl;

        /// button for pulling from usb
        line_usb = new Button(chip, 6);
        std::cout << "bluetooth pairing pin acquired" << std::endl;

        /// button for iterating to next led setting
        line_next = new Button(chip, 4);
        std::cout << "Previous setting pin acquired" << std::endl;

        /// button for iterating to previous led setting
        line_previous = new Button(chip, 5);
        std::cout << "Next setting pin acquired" << std::endl;

        /// button for enabling/ disabling the cube
        line_power = new Button(chip, 7);
        std::cout << "power on/ off pin acquired" << std::endl;
#pragma endregion
#pragma endregion
    }

    ~Main()
    {
        delete line_usb;
        delete line_previous;
        delete line_next;
        delete line_power;
    }

private:
    void init_fs()
    {
        // create directories if not existent
        if (not fs::exists(FileTransfer::default_path))
        {
            fs::create_directories(FileTransfer::default_path);
        }

        // create directories if not existent
        if (not fs::exists(FileTransfer::custom_path))
        {
            fs::create_directories(FileTransfer::custom_path);
        }
    }

    void parse_layout()
    {
        std::vector<Frame> _frames;

        // load from file
        std::ifstream stream(current_file);

        nlohmann::json file;
        stream >> file;

        // parse json
        for (auto &_frame: file["frames"])
        {
            Frame frame{};

            // frame time
            frame.frame_time = _frame["frame-time"];

            // layer data
            for (int i = 0; i < 5; ++i)
            {
                for (int j = 0; j < 5; ++j)
                {
                    for (int k = 0; k < 5; ++k)
                    {
                        const auto &value = _frame["layers"][i][j][k];
                        frame.data[i][j][k] = value;
                    }
                }
            }

            _frames.push_back(frame);
        }

        frames = _frames;
    }

#pragma region line controll abstraction methods
    void reset()
    {
        pin_reset.set_value(0);
        pin_reset.set_value(1);

        // set special pin for turn off on store
        pin_special_val = false;
    }

    void shift()
    {
        pin_shift.set_value(1);
        pin_shift.set_value(0);
    }

    void store()
    {
        // actually turn of reset pin
        if (pin_special_val)
        {
            pin_special.set_value(1);
        }
        else
        {
            pin_special.set_value(0);
        }

        pin_store.set_value(1);
        pin_store.set_value(0);
    }
#pragma endregion

    // todo this can lead to problems with threads
    void update_file_list()
    {
        std::string tmp = current_file;

        files.clear();

        // search all new files

        // add default cube configurations
        for (const auto &file: fs::recursive_directory_iterator(FileTransfer::default_path))
        {
            std::cout << "Found file: " << file.path() << std::endl;
            files.push_back(file.path());
        }
        // add custom cube configurations
        for (const auto &file: fs::recursive_directory_iterator(FileTransfer::custom_path))
        {
            std::cout << "Found file: " << file.path() << std::endl;
            files.push_back(file.path());
        }

        // search element in list, if not exits use fallback option
        current_file = files[0]; // fallback option
        for (const auto &val: files)
        {
            if (val == tmp)
            {
                current_file = val;
                break;
            }
        }
    }

    void del()
    {
        // if only one or fewer elements in list => no save deletion possible
        if (files.size() <= 1)
        {
            std::cout << "Only one config present: " << std::endl;
            return;
        }

        // if file is in default dir, then it cannot be deleted
        if (current_file.starts_with(FileTransfer::default_path.string()))
        {
            // todo use cerr
            std::cout << "File cannot be deleted, because it is in default dir" << std::endl;
            return;
        }

        // search element in list, if not exits use fallback option
        std::string tmp = current_file;

        // set current file to next elem
        next();

        try
        {
            fs::remove(tmp);
        }
        catch (std::exception &e)
        {
            // todo use cerr
            std::cout << "Error while deleting file: " << e.what() << std::endl;
            return;
        }

        std::cout << "New Configuration: " << current_file << std::endl;
    }

    void next()
    {
        // if only one or fewer elements in list => no scrolling possible
        if (files.size() <= 1)
        {
            std::cout << "Only one config present: " << std::endl;
            return;
        }

        // search element in list, if not exits use fallback option
        std::string tmp = current_file;
        current_file = files[0]; // fallback option
        for (int i = 0; i < files.size(); ++i)
        {
            const auto &it = files[i];

            if (it == tmp)
            {
                // when at end, the next element is the beginning of the list
                if (i == files.size()-1)
                {
                    // current_file = files[0]; // this is the fallback option, therefore change nothing
                }
                // when found use previous element as new one
                else
                {
                    current_file = files[i+1];
                }
                break;
            }
        }
        std::cout << "New Configuration: " << current_file << std::endl;
    }

    void previous()
    {
        // if only one or fewer elements in list => no scrolling possible
        if (files.size() <= 1)
        {
            std::cout << "Only one config present: " << std::endl;
            return;
        }

        // search element in list, if not exits use fallback option
        std::string tmp = current_file;
        current_file = files[0]; // fallback option
        for (int i = 0; i < files.size(); ++i)
        {
            const auto &it = files[i];

            if (it == tmp)
            {
                // when at beginning, the previous element is the beginning of the list
                if (i == 0)
                {
                    current_file = files[files.size()-1];
                }
                // when found use previous element as new one
                else
                {
                    current_file = files[i-1];
                }
                break;
            }
        }
        std::cout << "New Configuration: " << current_file << std::endl;
    }

    /**
     * Polls for button events and then process these events
     * @return true if one of the next or previous buttons have been pressed.
     */
    bool poll()
    {
        bool skip_frame_flag = false;

        line_usb->poll([&]()
            {
                std::cout << "file transfer button pressed" << std::endl;
                try
                {
                    FileTransfer ft(&line_blink_led);
                    ft.copy();
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Error while transferring: " << e.what() << std::endl;
                }

                // update the file, to load newly added files
                update_file_list();

                // todo maybe make ft pointer, so delete can be called explicitly
            },
            std::chrono::milliseconds(5), [&](){
                std::cout << "delete button triggered" << std::endl;
                del();
                update_file_list();
                parse_layout();
                skip_frame_flag = true;
            }
        );

        line_previous->poll([&]()
        {
            std::cout << "previous setting button press" << std::endl;
            previous();
            parse_layout();
            skip_frame_flag = true;
        });

        line_next->poll([&](){
            std::cout << "next setting button press" << std::endl;
            next();
            parse_layout();
            skip_frame_flag = true;
        });

        line_power->poll([&](){
            std::cout << "switch on/ off button press" << std::endl;
            reset();
            store();
            cube_on = not cube_on;
        });

        return skip_frame_flag;
    }

    void set_leds(const layers_t &frame_data)
    {
        if (not cube_on)
        {
            return;
        }

        for (int i = 0; i < frame_data.size(); ++i)
        {
            // reset all leds for next frame
            reset();

            for (int j = frame_data[i].size()-1; j >= 0; --j)
            {
                for (int k = frame_data[i][j].size()-1; k >= 0; --k)
                {
                    const auto led_value = frame_data[i][k][j];

                    // turn on special pin if end of shift register reached (layer 5 and pin 25)
                    if (j == 4 && k == 4)
                    {
                        pin_special_val = led_value;
                    }
                    else
                    {
                        pin_datain.set_value(led_value);
                        shift(); // only shift, when not the last pin
                    }
                }
            }

            // disable previous layer (no more than one layer is allowed to be on)
            if (i == 0) // if at the beginning of the array disable previous layer
            {
                layers[frame_data.size()-1].set_value(0);
            }
            else
            {
                layers[i-1].set_value(0);
            }

            store(); // store each layer

            // enable current layer
            layers[i].set_value(1);
        }
    }

public:
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
                if (poll())
                {
                    return;
                }

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
