#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <color.hpp>
#include <vec.hpp>

namespace nano
{

struct vertex
{
    vec2f pos;
    vec3f color;
    vec2f tpos;

    // operator vec2i() const;
    // operator struct color() const;
};

} // namespace nano

#endif // VERTEX_HPP
