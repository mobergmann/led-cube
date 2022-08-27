#ifndef CUBE_H
#define CUBE_H

#include <array>
#include "Led.h"

using std::array;


struct Cube
{
    array<array<array<Led, 5>, 5>, 5> leds;
};

#endif // CUBE_H
