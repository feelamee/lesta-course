#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>

#include <cstddef> // for size_t
#include <tuple>   // for pair

#include "color.hpp"

using position = std::pair<size_t, size_t>;
using namespace boost::numeric;

template <typename compT = uint8_t>
class canvas
{
public:
  using color_t = color<compT>;

  canvas (size_t p_width, size_t p_height,
          const color_t &p_color = color_black)
      : buf (p_width, p_height, p_color)
  {
  }

  canvas () {}

  inline size_t
  width () const
  {
    return buf.size1 ();
  }

  inline void
  resize (size_t width, size_t height, bool preserve = true)
  {
    buf.resize (width, height, preserve);
  }

  inline size_t
  height () const
  {
    return buf.size2 ();
  }

  inline const color_t &
  operator() (size_t i, size_t j) const
  {
    return buf (i, j);
  }

  inline color_t &
  operator() (size_t i, size_t j)
  {
    return buf (i, j);
  }

  inline void
  fill (const color_t &val)
  {
    std::fill (begin (buf.data ()), end (buf.data ()), val);
  }

  inline auto
  sub (ublas::slice p_slice1, ublas::slice p_slice2)
  {
    return ublas::project (buf, p_slice1, p_slice2);
  }

  inline bool
  operator== (const canvas &other)
  {
    return std::equal (begin (buf.data ()), end (buf.data ()),
                       begin (other.buf.data ()), end (other.buf.data ()));
  }

  const auto &
  data () const
  {
    return buf.data ();
  }

  auto &
  data ()
  {
    return buf.data ();
  }

private:
  ublas::matrix<color_t, ublas::column_major> buf;
};

#endif // CANVAS_HPP