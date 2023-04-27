#include <fstream>
#include <thread>
using namespace std::literals::chrono_literals;

#include "image_loader.hpp"
#include "triangle_render.hpp"

int
main (int argc, char const *argv[])
{
  std::fstream dst ("../homework/04-render-basics/1-render-primitives/out.ppm",
                    std::ios::out);
  constexpr size_t w = 1280, h = 720;

  canvas img (w, h);
  triangle_render render (w, h, img);
  render.clear ();

  std::vector<position> triangles;
  const position center = { w / 2, h / 2 };
  size_t radius = h * 4 / 5 / 2;
  float tmp = 0;

  while (true)
    {
      tmp = tmp < 6.28 ? tmp + 6.28 / 36 : 0;
      for (float angle = tmp; angle < 6.28 + tmp; angle += 6.28 / 12)
        {
          triangles.push_back (center);
          triangles.push_back ({ center.first + radius * cos (angle),
                                 center.second + radius * sin (angle) });
          triangles.push_back (
              { center.first + radius * cos (angle + 2 * 3.14 / 12),
                center.second + radius * sin (angle + 2 * 3.14 / 12) });
        }

      render.draw (triangles, { 0, 0, rand () % 255 });
      ppm::dump<ppm::fmt::P6> (dst, img);
      dst << std::endl;
      triangles.clear ();
      render.clear ();
      std::this_thread::sleep_for (100ms);
    }

  return 0;
}
