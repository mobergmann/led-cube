//
// Created by mobergmann on 21.08.22.
//

#ifndef CUBE_H
#define CUBE_H

#include <array>


struct Point
{
    int x, y, z;
};

struct Cube
{
    std::array<std::array<std::array<Point, 5>, 5>, 5> points;
};


#endif //CUBE_H
