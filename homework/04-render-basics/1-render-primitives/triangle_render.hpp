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

    virtual std::vector<position> polyline(position p1,
                                           position p2,
                                           position p3);

    using line_render::draw;

    void draw(const std::vector<position>& vertices, color color_);

    void draw(const std::vector<position>& vertices,
              std::vector<size_t>          indexes,
              color                        color_);

    using line_render::line;

    void rasterize_bresenham_stupid(const std::vector<position>& vertices,
                                    color                        color_);

    static position interpolate(const position& p1,
                                const position& p2,
                                float           t);

    static vertex interpolate(const vertex& p1, const vertex& p2, float t);

    void rasterize(const vertex& v1, const vertex& v2);

    void rasterize(std::vector<vertex>& vertices);
    void rasterize(const std::vector<vertex>& vertices,
                   std::vector<size_t>        indices);

    // TODO: rewrite to integers (triangle borders looks terrible)
    void rasterize(const vertex& v1, const vertex& v2, const vertex& v3);
};

#endif // TRIANGLE_RENDER_HPP
