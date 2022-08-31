#include "../include/FileTransfer.h"

namespace mount
{
#include <sys/mount.h>
#include <errno.h>
#include <string.h>

extern int errno;
};

std::mutex FileTransfer::mutex = std::mutex();

const fs::path FileTransfer::base_path = fs::path(getenv("HOME")) / ".led-cube";

const fs::path FileTransfer::default_path = FileTransfer::base_path / "default";

const fs::path FileTransfer::custom_path = FileTransfer::base_path / "config";

const fs::path FileTransfer::usb_path = "/dev/sda1";

const fs::path FileTransfer::mount_path = "/mount/volume";

FileTransfer::FileTransfer(gpiod::line *blink_led) : blink_led(blink_led), terminate_thread(false)
{
    // singleton
    if (not mutex.try_lock())
    {
        throw std::runtime_error("File transfer already in process");
    }

    // begin blink thread
    blink_thread = new std::thread(FileTransfer::blink, blink_led, &terminate_thread);

    // mount usb
    if (mount::mount(usb_path.c_str(), mount_path.c_str(), "ext4", 0, "")) // todo really ext4?
    {
        mutex.unlock(); // manually unlock, cause destructor not called with throw in constructor
        throw std::runtime_error("error while mounting! errno: " + std::to_string(errno) + ": " + std::string(mount::strerror(errno)));
    }

    try
    {
        // create directories if not existent
        if (not fs::exists(base_path))
        {
            fs::create_directory(base_path);
        }

        // create directories if not existent
        if (not fs::exists(default_path))
        {
            fs::create_directory(default_path);
        }

        // create directories if not existent
        if (not fs::exists(custom_path))
        {
            fs::create_directory(custom_path);
        }
    }
    catch (const std::exception &e)
    {
        mutex.unlock(); // manually unlock, cause destructor not called with throw in constructor
        throw e;
    }
}

FileTransfer::~FileTransfer()
{
    // terminate blink thread
    terminate_thread = true;
    blink_thread->join();
    delete blink_thread;

    // umount usb
    mount::umount(mount_path.c_str());

    // unlock usb mutex, so another transfer can be made
    mutex.unlock();
}

void FileTransfer::copy()
{
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
        fs::copy(i, custom_path);
    }
}

void FileTransfer::blink(gpiod::line *blink_led, const bool *terminate)
{
    while (true)
    {
        // turn on led
        blink_led->set_value(1);

        if (*terminate)
        {
            return;
        }
        // wait
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // turn off led
        blink_led->set_value(0);

        if (*terminate)
        {
            return;
        }
        // wait
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
