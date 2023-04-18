#include <engine.hpp>
#include <iostream>

int main(int argc, char const *argv[])
{
    int exit_code = nano::iengine::instance().initialize();
    if (EXIT_FAILURE == exit_code)
        return EXIT_FAILURE;

    nano::iengine::instance().run();
    nano::iengine::instance().finalize();
    return EXIT_SUCCESS;
}