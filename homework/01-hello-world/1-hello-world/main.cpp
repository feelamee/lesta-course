#include <iostream>

int main()
{
    std::cout << "Hello, world" << std::endl;
    return std::cout.good() ? EXIT_SUCCESS : EXIT_FAILURE;
}