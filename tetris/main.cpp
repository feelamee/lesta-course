#include <nano/engine.hpp>
#include <nano/error.hpp>

#include <cstdlib>

int
main()
{
    auto& e = nano::engine::instance();
    int err_code = e.initialize(nano::engine::flag::all);
    ASSERT_ERROR(err_code, "Fail while initialization of engine");

    return EXIT_SUCCESS;
}
