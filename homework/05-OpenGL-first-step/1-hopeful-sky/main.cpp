// see ~/code/lesta-course/external/SDL/include/SDL3/SDL_stdinc.h line 795
#define SDL_FUNCTION_POINTER_IS_VOID_POINTER
#include <SDL3/SDL.h>

#include <assert.h>
#include <glad/glad.h>

#include "raii_wrapper.hpp"

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

#define TEST_SDL_ERROR(expr)                                                   \
    {                                                                          \
        if (!(expr))                                                           \
        {                                                                      \
            std::cerr << SDL_GetError() << std::endl;                          \
        }                                                                      \
    }

namespace std
{
template <typename T, typename retT>
shared_ptr(T* res, retT (*)(T*)) -> shared_ptr<T>;
} // namespace std

int
main()
{
    raii_wrapper<int, void> sdl_init(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS),
                                     SDL_Quit);
    ASSERT_SDL_ERROR(EXIT_SUCCESS == sdl_init.constructor_ret_v);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    constexpr size_t w = 720, h = 480;
    std::shared_ptr window(SDL_CreateWindow("test", w, h, SDL_WINDOW_OPENGL),
                           SDL_DestroyWindow);
    ASSERT_SDL_ERROR(nullptr != window.get());

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

    std::shared_ptr<void> context(
        SDL_GL_CreateContext(window.get()),
        [](SDL_GLContext context)
        {
            ASSERT_SDL_ERROR(EXIT_SUCCESS == SDL_GL_DeleteContext(context));
            return EXIT_SUCCESS;
        });
    ASSERT_SDL_ERROR(nullptr != context.get());

    GLint real_major_version{}, real_minor_version{};
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &real_major_version);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &real_minor_version);
    assert(real_major_version >= 2 && real_minor_version >= 0);

    assert(0 != gladLoadGLES2Loader(SDL_GL_GetProcAddress));

    SDL_Event ev;
    bool is_running{ true };
    while (is_running)
    {
        while (SDL_PollEvent(&ev))
        {
            switch (ev.type)
            {
            case SDL_EVENT_QUIT:
                is_running = false;
                break;

            default:
                break;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 1, 1);
        TEST_SDL_ERROR(EXIT_SUCCESS == SDL_GL_SwapWindow(window.get()));
    }

    return EXIT_SUCCESS;
}
