#ifndef LED_H
#define LED_H


struct Led
{
    double x, y, z;
    const static int radius;
    bool on;
};

const int Led::radius = 10;

#endif // LED_H
