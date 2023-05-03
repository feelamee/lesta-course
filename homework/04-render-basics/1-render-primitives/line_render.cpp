#include "line_render.hpp"

#include <ranges>

void
line_render::clear(color color_)
{
    buf.fill(color_);
}

canvas&
line_render::data()
{
    return buf;
}

std::vector<position>
line_render::line(position p1, position p2)
{
    using std::ranges::for_each;
    using std::views::iota;

    bool slope = false;
    if (abs(p2.x - p1.x) < abs(p2.y - p1.y))
    {
        std::swap(p1.x, p1.y);
        std::swap(p2.x, p2.y);
        slope = true;
    }

    if (p1.x > p2.x)
        std::swap(p1, p2);

    const int dx{ p2.x - p1.x };
    const int dy{ abs(p2.y - p1.y) };
    const int yi_step = p1.y > p2.y ? -1 : 1;
    int error{ 0 };
    std::vector<position> result;
    result.reserve(dx + dy + 1);

    int yi{ p1.y };
    int half_dx = dx / 2;
    for (auto xi : iota(p1.x, p2.x + 1))
    {
        result.push_back(slope ? position{ yi, xi } : position{ xi, yi });
        error += dy;
        if (error > half_dx)
        {
            yi += yi_step;
            error -= dx;
        }
    }

    return result;
}

void
line_render::draw(position p1, position p2, color p_color)
{
    auto&& smpl = line(p1, p2);
    for (auto [i, j] : smpl)
        buf(i, j) = p_color;
}

void
line_render::draw(const std::vector<vertex>& vertices)
{
    for (auto&& v : vertices)
    {
        position pos(v);
        color c{ program->fragment_shader(v) };
        buf(pos.x, pos.y) = c;
    }
}

position
line_render::interpolate(const position& p1, const position& p2, float t)
{
    return {
        static_cast<position_t>(std::lerp(p1.x, p2.x, t)),
        static_cast<position_t>(std::lerp(p1.y, p2.y, t)),
    };
}

vertex
line_render::interpolate(const vertex& p1, const vertex& p2, float t)
{
    using std::lerp;
    return {
        lerp(p1.x, p2.x, t),   lerp(p1.y, p2.y, t), lerp(p1.r, p2.r, t),
        lerp(p1.g, p2.g, t),   lerp(p1.b, p2.b, t), lerp(p1.tx, p2.tx, t),
        lerp(p1.ty, p2.ty, t),
    };
}

std::vector<vertex>
line_render::line(const vertex& v1, const vertex& v2)
{
    std::vector<vertex> result;
    auto p_count{ std::abs(v2.x - v1.x) };
    if (p_count < 1.f)
        return { v1 };

    result.reserve(p_count + 1);
    const auto t{ 1.f / (p_count + 1) };

    float ti{};
    for (size_t p{}; p < p_count + 1; ++p)
    {
        result.push_back(interpolate(v1, v2, ti));
        ti += t;
    }
    return result;
}
