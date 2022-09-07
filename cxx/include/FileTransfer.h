#ifndef LED_CUBE_FILETRANSFER_H
#define LED_CUBE_FILETRANSFER_H

#include <mutex>
#include <string>
#include <thread>
#include <filesystem>
#include <gpiod.hpp>

using std::string;
namespace fs = std::filesystem;


class FileTransfer
{
private:
    /// singleton mutex
    static std::mutex mutex;

    /// pointer to the blink led
    gpiod::line* blink_led;

    /// thread which makes the transferring led blink
    std::thread *blink_thread;
    /// boolean, if the blink thread should be terminated
    static bool terminate_thread;

public:
    /// path to the default directory
    const static fs::path default_path;

    /// path to the configuration directory
    const static fs::path custom_path;

    /// path to the usb folder
    const static fs::path usb_path;

    /// path to the mounted folder
    const static fs::path mount_path;

private:
    /**
     * Blinks the given led as long as terminate is not true
     * @param blink_led gpio line, which should be blinked
     * @param terminate pointer to the terminate boolean
     */
    static void blink(gpiod::line *blink_led);

public:
    FileTransfer() = delete;

    /**
     * Initializes the singleton. If an instance is already running, throws a `runtime_error`.
     * Checks if all config folders are present and created them if not.
     * Mounts the USB stick.
     * @param blink_led
     */
    FileTransfer(gpiod::line *blink_led);

    /**
     * Stops the blink and unmounts the USB stick and releases the singleton
     */
    virtual ~FileTransfer();

    /**
     * Copy all json files from the usb to locale storage,
     * Only the root folder will be scanned (not recursive)
     */
    void copy();
};

#endif //LED_CUBE_FILETRANSFER_H
