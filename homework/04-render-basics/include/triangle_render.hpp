#ifndef TRIANGLE_RENDER_HPP
#define TRIANGLE_RENDER_HPP

#include "canvas.hpp"
#include "line_render.hpp"
#include "vertex.hpp"

#include <algorithm>
#include <cmath>
#include <ranges>

class triangle_render : public line_render
{
public:
    triangle_render(canvas& buf)
        : line_render(buf)
    {
    }

    virtual std::vector<position>
    polyline(position p1, position p2, position p3);

    using line_render::draw;

    void
    draw(const std::vector<position>&, color);

    void
    draw(const std::vector<position>&, const std::vector<size_t>&, color);

    using line_render::line;

    void
    rasterize_bresenham_stupid(const std::vector<position>&, color);

    void
    rasterize(const vertex&, const vertex&);

    void
    rasterize(const std::vector<vertex>&);

    void
    rasterize(const std::vector<vertex>&, const std::vector<size_t>&);

    // TODO: rewrite to integers (triangle borders looks terrible)
    void
    rasterize(const vertex&, const vertex&, const vertex&);

    std::vector<vertex>
    get_triangle_vertices(const vertex& v1, const vertex& v2, const vertex& v3);
};

#endif // TRIANGLE_RENDER_HPP
