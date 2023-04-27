#include "canvas.hpp"
#include "image_loader.hpp"

#include <fstream>
#include <iostream>

#include <boost/numeric/ublas/matrix_proxy.hpp>
using namespace boost::numeric::ublas;

int
main (int argc, char const *argv[])
{
  std::fstream src (
      "../homework/04-render-basics/1-render-primitives/test.ppm");
  std::fstream dst (
      "../homework/04-render-basics/1-render-primitives/out.ppm");

  canvas img1;
  int ret = ppm::load (src, img1);
  if (ret == EXIT_FAILURE)
    {
      std::cerr << "file1 read error" << std::endl;
      return EXIT_FAILURE;
    }

  std::cout << img1(0, 0) << " " << img1(0, 1) << std::endl;
  std::cout << img1(1, 0) << " " << img1(1, 1) << std::endl;

  ret = ppm::dump<ppm::fmt::P6> (dst, img1);
  if (ret == EXIT_FAILURE)
    {
      std::cerr << "file write error" << std::endl;
      return EXIT_FAILURE;
    }

  dst.seekg (0);
  canvas img2;
  ret = ppm::load (dst, img2);
  if (ret == EXIT_FAILURE)
    {
      std::cerr << "file2 read error" << std::endl;
      return EXIT_FAILURE;
    }

  std::cout << (img1 == img2 ? "equal" : "not equal") << std::endl;
  return EXIT_SUCCESS;
}
