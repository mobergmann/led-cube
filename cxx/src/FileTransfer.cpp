#include "../include/FileTransfer.h"

#include <string>
#include <iostream>
#include <cstring>

namespace mount
{
#include <sys/mount.h>
#include <errno.h>

extern int errno;
};

std::mutex FileTransfer::mutex = std::mutex();

bool FileTransfer::terminate_thread = false;

const fs::path FileTransfer::default_path = "/usr/share/led-cube";

const fs::path FileTransfer::custom_path = "/var/lib/led-cube";

const fs::path FileTransfer::usb_path = "/dev/sda1";

const fs::path FileTransfer::mount_path = "/mnt";


void FileTransfer::copy(gpiod::line *blink_led)
{
#pragma region init
    // singleton
    if (not mutex.try_lock())
    {
        throw std::runtime_error("File transfer already in process");
    }

    // begin blink thread
    auto* blink_thread = new std::thread(FileTransfer::blink, blink_led);

    // mount usb
    if (mount::mount(usb_path.c_str(), mount_path.c_str(), "vfat", 0, ""))
    {
        mutex.unlock(); // manually unlock, because destructor not called with throw in constructor
        throw std::runtime_error("error while mounting! errno: " + std::to_string(errno) + ": " + std::string(std::strerror(errno)));
    }
#pragma endregion

#pragma region copy files
    /**
     * Get all json files from usb
     */

    std::vector<fs::path> configurations;
    for (const auto &item: fs::directory_iterator(mount_path))
    {
        // skip directory
        if (item.is_directory())
        {
            continue;
        }

        // add json files
        if (item.path().string().ends_with(".json"))
        {
            configurations.push_back(item.path());
        }
    }

    /**
     * Copy all files from usb to storage
     */

    for (const auto &i: configurations)
    {
        fs::copy(i, custom_path, fs::copy_options::overwrite_existing);
    }
#pragma endregion

#pragma region finalize
    std::cout << "Thread delete signal send" << std::endl;

    // terminate blink thread
    FileTransfer::terminate_thread = true;
    blink_thread->join();
    delete blink_thread;

    // umount usb
    if (mount::umount(mount_path.c_str()))
    {
        mutex.unlock(); // manually unlock, because destructor not called with throw in constructor
        throw std::runtime_error("error while unmounting! errno: " + std::to_string(errno) + ": " + std::string(std::strerror(errno)));
    }

    // unlock usb mutex, so another transfer can be made
    mutex.unlock();
#pragma endregion
}

void FileTransfer::blink(gpiod::line *blink_led)
{
    // blink for 1 second on and off.
    // If terminate flag set to true, then quit blinking
    bool led_val = true;
    while (not FileTransfer::terminate_thread)
    {
        // turn on led
        blink_led->set_value(led_val);

        // wait
        std::this_thread::sleep_for(std::chrono::seconds(1));

        led_val = not led_val;
    }

    // set terminate flag to default value
    FileTransfer::terminate_thread = false;
    // turn off led after blinking
    blink_led->set_value(0);

    std::cout << "quiting blink thread" << std::endl;
}
