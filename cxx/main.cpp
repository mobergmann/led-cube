#include <gpiod.hpp>

#include <array>
#include <iostream>

#define RASPI_GPIO_CHIP "gpiochip0"
#define SPECIAL_PIN_NUMBER 13
#define SPECIAL_PIN_NAME "GPIO13"
#define LAYER_NUMBER 20
#define LAYER_NAME "GPIO20"

int main()
{
    // init chip
    gpiod::chip chip(RASPI_GPIO_CHIP, gpiod::chip::OPEN_BY_NAME);
    
    // init layer
    gpiod::line layer1;
    layer1 = chip.get_line(LAYER_NUMBER);
    layer1.request({LAYER_NAME, gpiod::line_request::DIRECTION_OUTPUT, 0},0);
    std::cout << "Layer aquired" << std::endl;

    // init test LED
    gpiod::line ln_led;
    ln_led = chip.get_line(SPECIAL_PIN_NUMBER);
    ln_led.request({SPECIAL_PIN_NAME, gpiod::line_request::DIRECTION_OUTPUT, 0},0);
    std::cout << "Led aquired" << std::endl;

    // set layer on
    layer1.set_value(1);
    
    while (true) {
        // set to on
        ln_led.set_value(1);

        // wait
        std::cout << "(Currently on) Press to Continue" << std::endl;
        std::cin.ignore();

        // set to off
        ln_led.set_value(0);
        
        // wait
        std::cout << "(Currently off) Press to Continue" << std::endl;
        std::cin.ignore();
    }
}
