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
};

static_assert(sizeof(vertex) == 7 * sizeof(float));

} // namespace nano

#endif // VERTEX_HPP
