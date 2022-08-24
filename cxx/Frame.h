#ifndef LED_CUBE_FRAME_H
#define LED_CUBE_FRAME_H

#include <array>


using values_t = std::array<bool, 5>;
using lines_t = std::array<values_t, 5>;
using layers_t = std::array<lines_t, 5>;

struct Frame
{
    /// the time in milliseconds, how long the frame should be visible
    unsigned int frame_time;
    /// for each layer a stream of booleans, encoding which led is on
    layers_t data;
};

#endif //LED_CUBE_FRAME_H
