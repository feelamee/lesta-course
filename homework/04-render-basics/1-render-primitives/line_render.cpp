#include "line_render.hpp"

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
    result.reserve(dx);

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
