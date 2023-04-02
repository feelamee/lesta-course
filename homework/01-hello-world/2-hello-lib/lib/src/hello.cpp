#include <iostream>
#include <cstdlib>
#include <hello.hpp>

namespace hw {

int hello()
{
    std::cout << "Hello, World!" << std::endl;
    return std::cout.good() ? EXIT_SUCCESS : EXIT_FAILURE;
}

}