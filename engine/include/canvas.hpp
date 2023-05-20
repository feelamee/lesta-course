#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <cstddef> // for size_t

#include <algorithm>
#include <color.hpp>
#include <vec.hpp>
#include <vector>

#include <iostream>

namespace nano
{

class canvas
{
public:
    canvas(vec2<std::size_t> size, const color& p_color = colors::black)
        : buf(size.fst * size.snd, p_color)
        , m_width(size.fst)
        , m_height(size.snd)
    {
    }

    canvas()
    {
    }

    inline size_t
    width() const
    {
        return m_width;
    }

    inline size_t
    height() const
    {
        return m_height;
    }

    inline vec2s
    size() const
    {
        return { m_width, m_height };
    }

    inline const color&
    operator()(size_t i, size_t j) const
    {
        return buf[i * width() + j];
    }

    inline color&
    operator()(size_t i, size_t j)
    {
        return buf[i * width() + j];
    }

    inline void
    fill(const color& val)
    {
        std::ranges::fill(buf, val);
    }

    inline bool
    operator==(const canvas& other)
    {
        return std::ranges::equal(buf, other.buf);
    }

    const color*
    data() const
    {
        return buf.data();
    }

    color*
    data()
    {
        return buf.data();
    }

    void
    resize(vec2s size)
    {
        m_width = size.fst;
        m_height = size.snd;
        buf.resize(size.fst * size.snd);
    }

    void
    transpose()
    {
        // can be better if use swap without creating new matrix
        std::vector<color> ret(width() * height());
        for (std::size_t i{ 0 }; i < width(); ++i)
            for (std::size_t j{ 0 }; j < height(); ++j)
            {
                ret[i * height() + j] = buf[j * width() + i];
            }
        buf = ret;
        std::swap(m_width, m_height);

        //                    00
        //  0010   00100000   00 00001000
        //  0000              10
        //                    00
    }

private:
    std::size_t m_width;
    std::size_t m_height;
    std::vector<color> buf;
};

} // namespace nano

#endif // CANVAS_HPP
