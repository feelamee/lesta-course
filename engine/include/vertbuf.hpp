#ifndef VERTBUF_HPP
#define VERTBUF_HPP

#include <vec.hpp>
#include <vector>
#include <vertex.hpp>

namespace nano
{

enum class primitive_t
{
    points,
    lines,
    line_loop,
    line_strip,
    triangles,
    triangle_strip,
    triangle_fan,
};

class vertbuf
{
    vec2i size();
    primitive_t primitive_type();
    vertex* data();

private:
    std::vector<vertex> vertices;
    vec2i m_size;
    primitive_t type;
};

} // namespace nano

#endif // VERTBUF_HPP
