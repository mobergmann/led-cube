//
// Created by mobergmann on 17.08.22.
//

#include "../include/Bluetooth.h"

Bluetooth::Bluetooth() {
    if (not m.try_lock())
    {
        throw std::runtime_error("Bluetooth still locked");
    }
}

Bluetooth::~Bluetooth() {
    m.unlock();
}

void Bluetooth::blink() {

}

void Bluetooth::scan() {

}

void Bluetooth::connect() {

}

void Bluetooth::idle() {

}

void Bluetooth::receive() {

}

void Bluetooth::save() {

}

void Bluetooth::disconnect() {

}

