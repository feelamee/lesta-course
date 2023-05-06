#include <SDL3/SDL.h>

// #include "raii_wrapper.hpp"

#include <cstdlib>
#include <iostream>
#include <memory>

#define ASSERT_SDL_ERROR(expr)                                                 \
    {                                                                          \
        if (!(expr))                                                           \
        {                                                                      \
            std::cerr << SDL_GetError() << std::endl;                          \
            return EXIT_FAILURE;                                               \
        }                                                                      \
    }

namespace std
{
template <typename T>
shared_ptr(T* res, void (*deleter)(T*)) -> shared_ptr<T>;
} // namespace std

int
main()
{
    // raii_wrapper<int, void> sdl_init(
    //     SDL_Init(0 /*SDL_INIT_VIDEO | SDL_INIT_EVENTS*/), []() {});
    // ASSERT_SDL_ERROR(EXIT_SUCCESS == sdl_init.constructor_ret_v);

    ASSERT_SDL_ERROR(EXIT_SUCCESS ==
                     SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS));
    SDL_Quit();

    return EXIT_SUCCESS;
}
