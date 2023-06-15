#include <nano/engine.hpp>

#include <nano/error.hpp>
#include <nano/event.hpp>
#include <nano/shader.hpp>
#include <nano/transform.hpp>
#include <nano/vertex.hpp>

#define SDL_FUNCTION_POINTER_IS_VOID_POINTER
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

#include <assert.h>
#include <chrono>
#include <cstdlib>
#include <memory>

namespace nano
{

struct engine::impl_t
{
    SDL_Window* window;
    void* context;

    ~impl_t()
    {
        SDL_DestroyWindow(window);
        if (context)
        {
            TEST_SDL_ERROR(EXIT_SUCCESS == SDL_GL_DeleteContext(context));
        }
        SDL_Quit();
    }
};

int
engine::initialize(int init_flags)
{
    flags = init_flags;

    impl = std::make_shared<impl_t>();
    int err_code = SDL_Init(init_flags);
    ASSERT_SDL_ERROR(EXIT_SUCCESS == err_code);

    if (not(flag::video & flags))
    {
        return EXIT_SUCCESS;
    }

    impl->window = SDL_CreateWindow(
        "test", window.size.x, window.size.y, SDL_WINDOW_OPENGL);
    ASSERT_SDL_ERROR(nullptr != impl->window);

    // TODO: extract such values to config
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

#ifdef __WINDOWS__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#endif

    GLint real_major_version{}, real_minor_version{};
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &real_major_version);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &real_minor_version);

    // TODO: extract such values to config
    assert(real_major_version == 2 && real_minor_version == 1);

    impl->context = SDL_GL_CreateContext(impl->window);
    ASSERT_SDL_ERROR(nullptr != impl->context);

    assert(0 != gladLoadGLES2Loader(SDL_GL_GetProcAddress));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplSDL3_InitForOpenGL(impl->window, impl->context);
    ImGui_ImplOpenGL3_Init("#version 300 es");

    return EXIT_SUCCESS;
}

void
engine::new_frame()
{
    if (not(flag::video & flags))
        return;

    ImGui_ImplSDL3_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
}

void
engine::renderUI()
{
    if (not(flag::video & flags))
        return;

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void
engine::finalize()
{
    if (not(flag::video & flags))
        return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

engine::~engine()
{
}

int
engine::swap_buffers()
{
    TEST_SDL_ERROR(EXIT_SUCCESS == SDL_GL_SwapWindow(impl->window));
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
    return EXIT_SUCCESS;
}

engine&
engine::instance()
{
    static engine nano{};
    return nano;
}

} // namespace nano
