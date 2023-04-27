#include "canvas.hpp"
#include "image_loader.hpp"
#include "line_render.hpp"

#include <boost/numeric/ublas/matrix.hpp>
using namespace boost::numeric;

#include <fstream>

template <typename compT>
line_render (size_t, size_t, canvas<compT> &) -> line_render<compT>;

int
main (int argc, char const *argv[])
{

  std::fstream dst ("../homework/04-render-basics/1-render-primitives/out.ppm",
                    std::ios::out);

  constexpr size_t w = 1280, h = 720;
  canvas img (w, h);

  line_render render (img.width (), img.height (), img);
  render.clear ();

  for (size_t i = 0; i < 300; ++i)
    {
      position start{ rand () % img.width (), rand () % img.height () };
      position end{ rand () % img.width (), rand () % img.height () };
      render.draw (start, end,
                   { static_cast<uint8_t> (rand () % 256),
                     static_cast<uint8_t> (rand () % 256),
                     static_cast<uint8_t> (rand () % 256) });
    }

  ppm::dump<ppm::fmt::P3> (dst, img);

  return 0;
}
