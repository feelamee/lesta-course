#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <color.hpp>

#include <algorithm>
#include <stdint.h>
#include <vector>

namespace nano
{

class canvas
{
public:
    canvas(std::size_t width,
           std::size_t height,
           const color& p_color = colors::black)
        : buf(width * height, p_color)
        , m_width(width)
        , m_height(height)
    {
    }

    canvas() = default;

    std::size_t width() const;
    std::size_t height() const;
    const color& operator()(size_t row, size_t col) const;
    color& operator()(size_t row, size_t col);
    void fill(const color& val);
    bool operator==(const canvas& other) const;
    const color* data() const;
    color* data();
    void resize(std::size_t width, std::size_t height);
    void transpose();

private:
    std::vector<color> buf;
    std::size_t m_width{ 0 };
    std::size_t m_height{ 0 };
};

} // namespace nano

#endif // CANVAS_HPP
