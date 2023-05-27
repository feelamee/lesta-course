#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <nano/color.hpp>
#include <nano/vec.hpp>

namespace nano
{

struct vertex
{
    vec2f pos;
    color rgb;
    vec2f tpos;
    // vec3f color;
};

// static_assert(sizeof(vertex) == 7 * sizeof(float));
static_assert(sizeof(vertex) == 20);

} // namespace nano

#endif // VERTEX_HPP
