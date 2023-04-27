#ifndef LINE_RENDER_HPP
#define LINE_RENDER_HPP

#include "canvas.hpp"
#include "render.hpp"

#include <boost/numeric/ublas/matrix.hpp>
#include <vector>
using namespace boost::numeric;

template <typename compT = uint8_t>
class line_render : render<compT>
{
private:
  canvas<compT> &buf;
  size_t width;
  size_t height;

  using color_t = color<compT>;

public:
  line_render (size_t _width, size_t _height, canvas<compT> &_buf)
      : width (_width), height (_height), buf (_buf)
  {
  }

  void
  clear (color_t color_ = color_black) override
  {
    buf.fill (color_);
  }

  canvas<compT> &
  data () override
  {
    return buf;
  }

  std::vector<position>
  line (position p1, position p2) override
  {
    std::vector<position> result;
    int x0 = p1.first;
    int y0 = p1.second;
    int x1 = p2.first;
    int y1 = p2.second;

    auto plot_line_low = [&] (int x0, int y0, int x1, int y1) {
      int dx = x1 - x0;
      int dy = y1 - y0;
      int yi = 1;
      if (dy < 0)
        {
          yi = -1;
          dy = -dy;
        }
      int D = 2 * dy - dx;
      int y = y0;

      for (int x = x0; x <= x1; ++x)
        {
          result.push_back (position{ x, y });
          if (D > 0)
            {
              y += yi;
              D -= 2 * dx;
            }
          D += 2 * dy;
        }
    };

    auto plot_line_high = [&] (int x0, int y0, int x1, int y1) {
      int dx = x1 - x0;
      int dy = y1 - y0;
      int xi = 1;
      if (dx < 0)
        {
          xi = -1;
          dx = -dx;
        }
      int D = 2 * dx - dy;
      int x = x0;

      for (int y = y0; y <= y1; ++y)
        {
          result.push_back (position{ x, y });
          if (D > 0)
            {
              x += xi;
              D -= 2 * dy;
            }
          D += 2 * dx;
        }
    };

    size_t dx = x1 - x0;
    size_t dy = y1 - y0;
    size_t bufsize = ceil (sqrt (dy * dy + dx * dx));
    result.reserve (bufsize);
    if (abs (y1 - y0) < abs (x1 - x0))
      {
        if (x0 > x1)
          {
            plot_line_low (x1, y1, x0, y0);
          }
        else
          {
            plot_line_low (x0, y0, x1, y1);
          }
      }
    else
      {
        if (y0 > y1)
          {
            plot_line_high (x1, y1, x0, y0);
          }
        else
          {
            plot_line_high (x0, y0, x1, y1);
          }
      }
    return result;
  }

  void
  draw (position p1, position p2, color_t p_color) override
  {
    auto &&smpl = line (p1, p2);
    for (auto [i, j] : smpl)
      buf (i, j) = p_color;
  }
};

#endif // LINE_RENDER_HPP