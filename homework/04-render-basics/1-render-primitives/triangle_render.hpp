#ifndef TRIANGLE_RENDER_HPP
#define TRIANGLE_RENDER_HPP

#include "canvas.hpp"
#include "line_render.hpp"

#include <algorithm>
#include <ranges>

template <typename compT = uint8_t>
class triangle_render : public line_render<compT>
{
public:
  triangle_render (size_t width, size_t height, canvas<compT> &buf)
      : line_render<compT> (width, height, buf)
  {
  }

  // clang-format off
  virtual std::vector<position>
  polyline (position p1, position p2, position p3)
  {
    auto &&line1 = this->line (p1, p2);
    auto &&line2 = this->line (p1, p3);
    auto &&line3 = this->line (p2, p3);
    std::vector<position> result (line1.size () +
                                  line2.size () +
                                  line3.size ());

    using std::ranges::move_backward;
    move_backward (line1, result.begin ());
    move_backward (line2, result.begin () + line1.size ());
    move_backward (line3, result.begin () + line1.size () + line2.size ());

    return result;
  }
  // clang-format on

  using line_render<compT>::draw;

  void
  draw (const std::vector<position> &vertexes, color<compT> color_)
  {
    size_t size = vertexes.size ();

    size_t i = 0, j = 1, k = 2;
    while (i < size and j < size and k < size)
      {
        position a = vertexes[i];
        position b = vertexes[j];
        position c = vertexes[k];
        draw (a, b, color_);
        draw (b, c, color_);
        draw (c, a, color_);
        i += 3;
        j += 3;
        k += 3;
      }
  }

  void
  draw (const std::vector<position> &vertexes, std::vector<size_t> indexes,
        color<compT> color_)
  {
    size_t size = indexex.size ();

    size_t i = 0, j = 1, k = 2;
    while (i < size and j < size and k < size)
      {
        position a = vertexes.at (indexes[i]);
        position b = vertexes.at (indexes[j]);
        position c = vertexes.at (indexes[k]);
        draw (a, b, color_);
        draw (b, c, color_);
        draw (c, a, color_);
        i += 3;
        j += 3;
        k += 3;
      }
  }
};

#endif // TRIANGLE_RENDER_HPP