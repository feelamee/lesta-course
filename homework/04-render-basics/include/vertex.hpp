#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <color.hpp>
#include <vec.hpp>

struct vertex
{
    vec2f pos;
    color rgb;
    vec2f tpos;
};

static_assert(sizeof(vertex) == 20);

#endif // VERTEX_HPP
