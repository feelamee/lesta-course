#ifndef VERTBUF_HPP
#define VERTBUF_HPP

#include <nano/vec.hpp>
#include <nano/vertex.hpp>

#include <vector>

namespace nano
{

enum class primitive_t
{
    unknown = -1,
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
public:
    vertbuf(primitive_t p_type, const std::vector<vertex>& p_vertices);
    vertbuf() = default;

    std::size_t size() const;
    primitive_t primitive_type() const;
    void bind_vbo() const;
    void draw() const;

    vertex* data();
    const vertex* data() const;

private:
    std::vector<vertex> vertices;
    primitive_t type{ primitive_t::unknown };
};

void render(const vertbuf&);

} // namespace nano

#endif // VERTBUF_HPP
