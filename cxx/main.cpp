#include <iostream>
#include <gpiod.hpp>
#include <array>

#include <chrono>
#include <thread>

#define RASPI_GPIO_CHIP "gpiochip0"
#define SPECIAL_PIN_NUMBER 25
#define SPECIAL_PIN_NAME "GPIO25"

int main()
{
    // init chip
    gpiod::chip chip(RASPI_GPIO_CHIP, gpiod::chip::OPEN_BY_NAME);
    auto lines = chip.get_line(SPECIAL_PIN_NUMBER);

    // init test LED
    gpiod::line ln_led;
    ln_led=chip.get_line(25);
    ln_led.request({"GPIO25", gpiod::line_request::DIRECTION_OUTPUT, 0},0);
    std::cout<<"Led's line aquired.."<<::std::endl;

    while (true) {
        // set to on
        ln_led.set_value(1);

        // wait
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // set to off
        ln_led.set_value(0);
    }
}
