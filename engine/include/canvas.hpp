#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/storage.hpp>

#include <cstddef> // for size_t

#include <color.hpp>
#include <vec.hpp>

namespace nano
{

using namespace boost::numeric;

class canvas
{
public:
    canvas(vec2<std::size_t> size, const color& p_color = colors::black)
        : buf(size.fst, size.snd, p_color)
    {
    }

    canvas()
    {
    }

    inline size_t
    width() const
    {
        return buf.size1();
    }

    inline void
    resize(size_t width, size_t height, bool preserve = true)
    {
        buf.resize(width, height, preserve);
    }

    inline size_t
    height() const
    {
        return buf.size2();
    }

    inline const color&
    operator()(size_t i, size_t j) const
    {
        return buf(i, j);
    }

    inline color&
    operator()(size_t i, size_t j)
    {
        return buf(i, j);
    }

    inline void
    fill(const color& val)
    {
        std::fill(begin(buf.data()), end(buf.data()), val);
    }

    inline auto
    sub(ublas::slice p_slice1, ublas::slice p_slice2)
    {
        return ublas::project(buf, p_slice1, p_slice2);
    }

    inline bool
    operator==(const canvas& other)
    {
        return std::equal(begin(buf.data()),
                          end(buf.data()),
                          begin(other.buf.data()),
                          end(other.buf.data()));
    }

    const auto&
    data() const
    {
        return buf.data();
    }

    auto&
    data()
    {
        return buf.data();
    }

private:
    ublas::matrix<color, ublas::column_major> buf;
};

} // namespace nano

#endif // CANVAS_HPP
