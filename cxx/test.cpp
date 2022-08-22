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

namespace fs = std::filesystem;


struct Frame
{
    /// the time in milliseconds, how long the frame should be visible
    unsigned int frame_time;
    /// for each layer a stream of booleans, encoding which led is on
    layers_t data;
};


class A
{
public:
/// a list of frames, each representing a current state of the cube
std::vector<Frame> frames;

#pragma region file management
/// a list of all files in the data folder
std::vector<std::string> files;

/// stores the current configuration file
std::string current_file;
#pragma endregion


void poll(const std::function<void()>& callback)
{
    // run callback function
    callback();
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

        _frames.push_back(frame);
    }

    frames = _frames;
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

void next()
{
    // if only one or fewer elements in list => no scrolling possible
    if (files.size() <= 1)
    {
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


void pp(const std::vector<Frame>&)
{
    for (const auto &f: frames)
    {
        std::cout << "Frame" << std::endl;
        for (const auto &layer: f.data)
        {
            std::cout << "\tlayer:" << std::endl;
            for (const auto &line: layer)
            {
                std::cout << "\t\t";
                for (const auto &value: line)
                {
                    std::cout << value << ", ";
                }
                std::cout << std::endl;
            }
        }
        std::cout << std::endl << std::endl;
    }
//    std::cout << << std::endl;
}

};

int main()
{
    A m;
    m.update_file_list();

    for (int i = 0; i < 20; ++i)
    {
        m.poll([&](){
            m.previous();
            m.parse_layout();
            m.pp(m.frames);
        });
    }
}
