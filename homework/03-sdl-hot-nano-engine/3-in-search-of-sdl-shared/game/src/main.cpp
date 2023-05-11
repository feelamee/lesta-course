#include <engine.hpp>
#include <iostream>

int
main(int argc, char const* argv[])
{
    nano::iengine& nano = nano::engine_instance();

    int err_code = nano.initialize();
    if (err_code == EXIT_FAILURE)
    {
        std::cerr << "Engine init failed" << std::endl;
        return EXIT_FAILURE;
    }

    nano.run();
    nano.finalize();
    return EXIT_SUCCESS;
}