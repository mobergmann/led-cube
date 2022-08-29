//
// Created by mobergmann on 17.08.22.
//

#ifndef LED_CUBE_BLUETOOTH_H
#define LED_CUBE_BLUETOOTH_H

#include <mutex>


class Bluetooth
{
private:
    std::mutex m;
public:
    Bluetooth()
    {
        if (not m.try_lock())
        {
            throw std::runtime_error("Bluetooth still locked");
        }
    }

    ~Bluetooth()
    {
        m.unlock();
    }
};


#endif //LED_CUBE_BLUETOOTH_H
