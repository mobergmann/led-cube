#include <array>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <filesystem>

#include <gpiod.hpp>


std::vector<std::string> files;
std::string current_file;


void update_file_list()
{
    std::string tmp = current_file;

    files.clear();

    // search all new files
    std::string data_dir_path = "/home/mobergmann/Downloads/files";
    for (const auto &file: std::filesystem::recursive_directory_iterator(data_dir_path))
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
}


int main()
{
    update_file_list();

    std::cout << "start:  " << current_file << std::endl;

    next();
    std::cout << "next: " << current_file << std::endl;
    next();
    std::cout << "next: " << current_file << std::endl;
    next();
    std::cout << "next: " << current_file << std::endl;
    next();
    std::cout << "next: " << current_file << std::endl;


    previous();
    std::cout << "next: " << current_file << std::endl;
    previous();
    std::cout << "next: " << current_file << std::endl;
    previous();
    std::cout << "next: " << current_file << std::endl;

    previous();
    std::cout << "next: " << current_file << std::endl;
}
