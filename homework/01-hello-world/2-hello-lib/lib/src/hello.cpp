#include <cstdlib>
#include <hello.hpp>
#include <iostream>

namespace hw
{

int hello()
{
    if (!std::cout.fail())
        std::cout << "Hello, World!" << std::endl;

    return std::cout.good() ? EXIT_SUCCESS : EXIT_FAILURE;
}

} // namespace hw