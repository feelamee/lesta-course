#include <cstdlib>
#include <hello.hpp>
#include <iostream>

namespace hw
{

int hello()
{
    if (std::cout.bad())
        return EXIT_FAILURE;

    std::cout << "Hello, World!" << std::endl;

    return std::cout.good() ? EXIT_SUCCESS : EXIT_FAILURE;
}

} // namespace hw