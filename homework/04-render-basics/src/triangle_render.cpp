#include "triangle_render.hpp"
#include <array>
#include <bits/ranges_algobase.h>

std::vector<position>
triangle_render::polyline(position p1, position p2, position p3)
{
    auto&& line1 = this->line(p1, p2);
    auto&& line2 = this->line(p1, p3);
    auto&& line3 = this->line(p2, p3);

    std::vector<position> result(line1.size() + line2.size() + line3.size());

    using std::ranges::move_backward;
    move_backward(line1, result.end());
    move_backward(line2, result.end());
    move_backward(line3, result.end());

    return result;
}

void
triangle_render::draw(const std::vector<position>& vertices, color color_)
{
    size_t size = vertices.size();

    size_t i = 0, j = 1, k = 2;
    while (i < size and j < size and k < size)
    {
        position a = vertices[i];
        position b = vertices[j];
        position c = vertices[k];
        draw(a, b, color_);
        draw(b, c, color_);
        draw(c, a, color_);
        i += 3;
        j += 3;
        k += 3;
    }
}

void
triangle_render::draw(const std::vector<position>& vertices,
                      const std::vector<size_t>& indexes,
                      color color_)
{
    size_t size = indexes.size();

    size_t i = 0, j = 1, k = 2;
    while (i < size and j < size and k < size)
    {
        position a = vertices.at(indexes[i]);
        position b = vertices.at(indexes[j]);
        position c = vertices.at(indexes[k]);
        draw(a, b, color_);
        draw(b, c, color_);
        draw(c, a, color_);
        i += 3;
        j += 3;
        k += 3;
    }
}

void
triangle_render::rasterize_bresenham_stupid(
    const std::vector<position>& vertices, color color_)
{
    if (vertices.size() != 3)
        return;

    draw(vertices, color_);

    for (auto&& pos : line(vertices[0], vertices[2]))
        draw(line(vertices[1], pos), color_);
}

void
triangle_render::rasterize(const std::vector<vertex>& vertices)
{
    size_t i{ 0 }, j{ 1 }, k{ 2 };
    while (k < vertices.size())
    {
        rasterize(vertices[i], vertices[j], vertices[k]);
        i += 3, j += 3, k += 3;
    }
}

void
triangle_render::rasterize(const std::vector<vertex>& vertices,
                           const std::vector<size_t>& indices)
{
    size_t size = indices.size();

    size_t i = 0, j = 1, k = 2;
    while (k < size)
    {
        vertex a = vertices[indices[i]];
        vertex b = vertices[indices[j]];
        vertex c = vertices[indices[k]];
        rasterize(program->vertex_shader(a),
                  program->vertex_shader(b),
                  program->vertex_shader(c));
        i += 3;
        j += 3;
        k += 3;
    }
}

// TODO: rewrite to integers (triangle borders looks terrible)
void
triangle_render::rasterize(const vertex& v1, const vertex& v2, const vertex& v3)
{
    using namespace std::ranges;

    auto vert_y_less = [](const vertex& lhs, const vertex& rhs)
    { return lhs.pos.y < rhs.pos.y; };
    const auto& [v12min, v12max] = minmax(v1, v2, vert_y_less);
    const auto& [v12maxv3_min, v_ymax] = minmax(v12max, v3, vert_y_less);
    const auto& [v_ymin, v_ymid] = minmax(v12min, v12maxv3_min, vert_y_less);

    float part21 = 1. / (v_ymax.pos.y - v_ymid.pos.y);
    float part20 = 1. / (v_ymax.pos.y - v_ymin.pos.y);
    float part10 = 1. / (v_ymid.pos.y - v_ymin.pos.y);

    if (std::isinf(part20))
    {
        draw(line(v_ymin, v_ymid));
        draw(line(v_ymid, v_ymax));
        draw(line(v_ymin, v_ymax));
        return;
    }

    float t1{}, t2{};
    while (t1 < 1.)
    {
        draw(line(interpolate(v_ymax, v_ymid, t1),
                  interpolate(v_ymax, v_ymin, t2)));
        t1 += part21;
        t2 += part20;
    }

    t1 = 0;
    while (t1 < 1.)
    {
        draw(line(interpolate(v_ymid, v_ymin, t1),
                  interpolate(v_ymax, v_ymin, t2)));
        t1 += part10;
        t2 += part20;
    }
}

std::vector<vertex>
triangle_render::get_triangle_vertices(const vertex& v1,
                                       const vertex& v2,
                                       const vertex& v3)
{
    using namespace std::ranges;

    auto vert_y_less = [](const vertex& lhs, const vertex& rhs)
    { return lhs.pos.y < rhs.pos.y; };
    const auto& [v12min, v12max] = minmax(v1, v2, vert_y_less);
    const auto& [v12maxv3_min, v_ymax] = minmax(v12max, v3, vert_y_less);
    const auto& [v_ymin, v_ymid] = minmax(v12min, v12maxv3_min, vert_y_less);

    float part21 = 1. / (v_ymax.pos.y - v_ymid.pos.y);
    float part20 = 1. / (v_ymax.pos.y - v_ymin.pos.y);
    float part10 = 1. / (v_ymid.pos.y - v_ymin.pos.y);

    std::vector<vertex> vertices;
    if (std::isinf(part20))
    {
        move_backward(line(v_ymin, v_ymid), vertices.end());
        move_backward(line(v_ymid, v_ymax), vertices.end());
        move_backward(line(v_ymin, v_ymax), vertices.end());
        return vertices;
    }

    float t1{}, t2{};
    while (t1 < 1.)
    {
        move_backward(line(interpolate(v_ymax, v_ymid, t1),
                           interpolate(v_ymax, v_ymin, t2)),
                      vertices.end());
        t1 += part21;
        t2 += part20;
    }

    t1 = 0;
    while (t1 < 1.)
    {
        move_backward(line(interpolate(v_ymid, v_ymin, t1),
                           interpolate(v_ymax, v_ymin, t2)),
                      vertices.end());
        t1 += part10;
        t2 += part20;
    }
    return vertices;
}
