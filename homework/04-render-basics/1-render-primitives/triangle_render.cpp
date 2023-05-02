#include "triangle_render.hpp"
#include <array>

std::vector<position>
triangle_render::polyline(position p1, position p2, position p3)
{
    auto&& line1 = this->line(p1, p2);
    auto&& line2 = this->line(p1, p3);
    auto&& line3 = this->line(p2, p3);

    std::vector<position> result(line1.size() + line2.size() + line3.size());

    using std::ranges::move_backward;
    move_backward(line1, result.begin());
    move_backward(line2, result.begin() + line1.size());
    move_backward(line3, result.begin() + line1.size() + line2.size());

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
                      std::vector<size_t> indexes,
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

position
triangle_render::interpolate(const position& p1, const position& p2, float t)
{
    return {
        static_cast<position_t>(std::lerp(p1.x, p2.x, t)),
        static_cast<position_t>(std::lerp(p1.y, p2.y, t)),
    };
}

vertex
triangle_render::interpolate(const vertex& p1, const vertex& p2, float t)
{
    using std::lerp;
    return {
        lerp(p1.x, p2.x, t),   lerp(p1.y, p2.y, t), lerp(p1.r, p2.r, t),
        lerp(p1.g, p2.g, t),   lerp(p1.b, p2.b, t), lerp(p1.tx, p2.tx, t),
        lerp(p1.ty, p2.ty, t),
    };
}

void
triangle_render::rasterize(const vertex& v1, const vertex& v2)
{
    std::vector<vertex> result;
    auto p_count{ std::abs(v2.x - v1.x) };
    if (p_count < 1.f)
        return;

    result.reserve(p_count + 1);
    const auto t{ 1.f / (p_count + 1) };

    float ti{};
    for (size_t p{}; p < p_count + 1; ++p)
    {
        result.push_back(interpolate(v1, v2, ti));
        ti += t;
    }
    draw(result);
}

void
triangle_render::rasterize(std::vector<vertex>& vertices)
{
    if (vertices.size() % 3 != 0)
        return;

    size_t i{ 0 }, j{ 1 }, k{ 2 };
    for (; k < vertices.size();)
    {
        rasterize(vertices[i], vertices[j], vertices[k]);
        i += 3, j += 3, k += 3;
    }
}

void
triangle_render::rasterize(const std::vector<vertex>& vertices,
                           std::vector<size_t> indices)
{
    size_t size = indices.size();

    size_t i = 0, j = 1, k = 2;
    while (k < size)
    {
        vertex a = vertices.at(indices[i]);
        vertex b = vertices.at(indices[j]);
        vertex c = vertices.at(indices[k]);
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

    const auto& [v12min, v12max] = minmax(v1, v2, {}, &vertex::y);
    const auto& [v12maxv3_min, v_ymax] = minmax(v12max, v3, {}, &vertex::y);
    const auto& [v_ymin, v_ymid] = minmax(v12min, v12maxv3_min, {}, &vertex::y);

    float part21 = 1. / (v_ymax.y - v_ymid.y);
    float part20 = 1. / (v_ymax.y - v_ymin.y);
    float part10 = 1. / (v_ymid.y - v_ymin.y);

    if (std::isinf(part20))
    {
        rasterize(v_ymin, v_ymid);
        rasterize(v_ymid, v_ymax);
        rasterize(v_ymin, v_ymax);
        return;
    }

    float t1{}, t2{};
    while (t1 < 1.)
    {
        rasterize(interpolate(v_ymax, v_ymid, t1),
                  interpolate(v_ymax, v_ymin, t2));
        t1 += part21;
        t2 += part20;
    }

    t1 = 0;
    while (t1 < 1.)
    {
        rasterize(interpolate(v_ymid, v_ymin, t1),
                  interpolate(v_ymax, v_ymin, t2));
        t1 += part10;
        t2 += part20;
    }
}
