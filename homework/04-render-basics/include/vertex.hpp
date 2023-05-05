#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "color.hpp"
#include <cmath>
#include <initializer_list>

using position_t = int;

struct vertex;
struct position
{
    position_t x;
    position_t y;

    operator vertex();
};

using coordinate_t = double;

struct vertex
{
    coordinate_t x;
    coordinate_t y;

    float r;
    float g;
    float b;

    float tx;
    float ty;

    operator position() const;
    operator color() const;
};

#endif // VERTEX_HPP
