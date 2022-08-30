//
// Created by mobergmann on 17.08.22.
//

#ifndef LED_CUBE_BLUETOOTH_H
#define LED_CUBE_BLUETOOTH_H

#include <mutex>
#include <thread>


class Bluetooth
{
private:
    std::mutex m;

    std::thread blink_thread;
    bool terminate_blink_thread = false;

public:
    Bluetooth();

    ~Bluetooth();

private:
    void blink();

    // void ();

public:
    void scan();

    void connect();

    void idle();

    void receive();

    void save();

    void disconnect();

};


#endif //LED_CUBE_BLUETOOTH_H
