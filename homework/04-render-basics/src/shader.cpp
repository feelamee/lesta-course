#include "shader.hpp"

vertex
texture_shader::vertex_shader(const vertex& v)
{
    return v;
}

color
texture_shader::fragment_shader(const vertex& v)
{
    auto width = (*uniform.buf).width() - 1;
    auto height = (*uniform.buf).height() - 1;
    color result = (*uniform.buf)(static_cast<position_t>(width * v.tx),
                                  static_cast<position_t>(height * v.ty));
    return result;
}

vertex
blackwhite::vertex_shader(const vertex& v)
{
    return v;
}

color
blackwhite::fragment_shader(const vertex& v)
{
    auto avg = (v.r + v.g + v.b) / 3;

    auto max = std::numeric_limits<color_channel_t<>>::max();
    return { static_cast<color_channel_t<>>(avg * max),
             static_cast<color_channel_t<>>(avg * max),
             static_cast<color_channel_t<>>(avg * max) };
}

vertex
funny_moment::vertex_shader(const vertex& v)
{
    return v;
}

color
funny_moment::fragment_shader(const vertex& v)
{
    const auto width = (*uniform.buf).width() - 1;
    const auto height = (*uniform.buf).height() - 1;
    const auto x = static_cast<position_t>(v.tx * width);
    const auto y = static_cast<position_t>(v.ty * height);
    const int dx = v.x - uniform.mouse_x;
    const int dy = v.y - uniform.mouse_y;
    float len_to_mouse = std::sqrt(dx * dx + dy * dy);

    float ratio = len_to_mouse / uniform.radius;
    if (uniform.radius < 0.001 or ratio < 0.001)
        ratio = std::numeric_limits<float>::max();

    size_t res_x = std::clamp(static_cast<size_t>(x - dx / ratio),
                              static_cast<size_t>(0),
                              static_cast<size_t>(width));
    size_t res_y = std::clamp(static_cast<size_t>(y - dy / ratio),
                              static_cast<size_t>(0),
                              static_cast<size_t>(height));
    color result = (*uniform.buf)(res_x, res_y);

    return result;
}

vertex
lupa::vertex_shader(const vertex& v)
{
    return v;
}

color
lupa::fragment_shader(const vertex& v)
{
    const auto width = (*uniform.buf).width() - 1;
    const auto height = (*uniform.buf).height() - 1;
    const auto x = static_cast<position_t>(v.tx * width);
    const auto y = static_cast<position_t>(v.ty * height);
    const int dx = v.x - uniform.mouse_x;
    const int dy = v.y - uniform.mouse_y;
    float len_to_mouse = std::sqrt(dx * dx + dy * dy);

    color result = (*uniform.buf)(x, y);
    if (std::abs(len_to_mouse - uniform.radius) < 2)
    {
        result.r = 0;
        result.g = 0;
        result.b = 0;
    }
    if (len_to_mouse < uniform.radius)
    {
        size_t res_x =
            std::clamp((int)(x - dx * uniform.scale), 0, (int)(width - 1));
        size_t res_y =
            std::clamp((int)(y - dy * uniform.scale), 0, (int)(height - 1));
        result = (*uniform.buf)(res_x, res_y);
    }

    return result;
}

vertex
blur::vertex_shader(const vertex& v)
{
    return v;
}

color
blur::fragment_shader(const vertex& v)
{
    const auto width = (*uniform.buf).width() - 1;
    const auto height = (*uniform.buf).height() - 1;
    const auto x = static_cast<position_t>(v.tx * width);
    const auto y = static_cast<position_t>(v.ty * height);
    const int dx = v.x - uniform.mouse_x;
    const int dy = v.y - uniform.mouse_y;
    float len_to_mouse = std::sqrt(dx * dx + dy * dy);

    color result = (*uniform.buf)(x, y);
    if (len_to_mouse < uniform.radius)
    {
        float r{}, g{}, b{};
        const size_t i_ =
            std::clamp((int)(x - uniform.strength), 0, (int)(width - 1));
        const size_t j_ =
            std::clamp((int)(y - uniform.strength), 0, (int)(height - 1));
        const size_t max_i =
            std::clamp((int)(x + uniform.strength), 0, (int)(width - 1));
        const size_t max_j =
            std::clamp((int)(y + uniform.strength), 0, (int)(height - 1));

        for (size_t i{ i_ }; i < max_i; ++i)
            for (size_t j{ j_ }; j < max_j; ++j)
            {
                // TODO: get directly row to increase perfomance
                auto& v = (*uniform.buf)(i, j);
                r += v.r;
                g += v.g;
                b += v.b;
            }

        const auto considered_pixels = (max_i - i_) * (max_j - j_);
        result.r = r / considered_pixels;
        result.g = g / considered_pixels;
        result.b = b / considered_pixels;
    }

    return result;
}
