#include <nano/canvas.hpp>

namespace nano
{

std::size_t
canvas::width() const
{
    return m_width;
}

std::size_t
canvas::height() const
{
    return m_height;
}

const color&
canvas::operator()(size_t row, size_t col) const
{
    return buf[row * width() + col];
}

color&
canvas::operator()(size_t row, size_t col)
{
    return buf[row * width() + col];
}

void
canvas::fill(const color& val)
{
    std::ranges::fill(buf, val);
}

bool
canvas::operator==(const canvas& other) const
{
    return std::ranges::equal(buf, other.buf);
}

const color*
canvas::data() const
{
    return buf.data();
}

color*
canvas::data()
{
    return buf.data();
}

void
canvas::resize(std::size_t width, std::size_t height)
{
    m_width = width;
    m_height = height;
    buf.resize(m_width * m_height);
}

void
canvas::transpose()
{
    // TODO: can be better if use swap without creating new matrix
    std::vector<color> new_buf(width() * height());
    for (std::size_t i{ 0 }; i < width(); ++i)
        for (std::size_t j{ 0 }; j < height(); ++j)
        {
            new_buf[i * height() + j] = buf[j * width() + i];
        }
    buf = new_buf;
    std::swap(m_width, m_height);

    //                    00
    //  0010   00100000   00 00001000
    //  0000              10
    //                    00
}

} // namespace nano
