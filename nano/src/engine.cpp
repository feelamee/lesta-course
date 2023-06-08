#include <chrono>
#include <cstdlib>

#include <assert.h>
#include <iostream>
#include <memory>
#include <thread>

// see ~/code/lesta-course/external/SDL/include/SDL3/SDL_stdinc.h line 795
#define SDL_FUNCTION_POINTER_IS_VOID_POINTER
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

#include <nano/engine.hpp>
#include <nano/error.hpp>
#include <nano/event.hpp>
#include <nano/shader.hpp>
#include <nano/transform.hpp>
#include <nano/vertex.hpp>

namespace nano
{

struct engine::impl_t
{
    SDL_Window* window;
    void* context;

    ~impl_t()
    {
        SDL_DestroyWindow(window);
        TEST_SDL_ERROR(EXIT_SUCCESS == SDL_GL_DeleteContext(context));
        SDL_Quit();
    }
};

void*
engine::window()
{
    return impl->window;
}
void*
engine::context()
{
    return impl->context;
}

int
engine::initialize()
{
    impl = std::make_shared<impl_t>();
    int err_code = SDL_Init(SDL_INIT_EVERYTHING);
    ASSERT_SDL_ERROR(EXIT_SUCCESS == err_code);

    // TODO: extract such values to config
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    constexpr size_t w = 720, h = 480;

    impl->window = SDL_CreateWindow("test", w, h, SDL_WINDOW_OPENGL);
    ASSERT_SDL_ERROR(nullptr != impl->window);

    // TODO: extract such values to config
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

    impl->context = SDL_GL_CreateContext(impl->window);
    ASSERT_SDL_ERROR(nullptr != impl->context);

    GLint real_major_version{}, real_minor_version{};
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &real_major_version);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &real_minor_version);

    // TODO: extract such values to config
    assert(real_major_version == 3 && real_minor_version == 2);

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
    ImGui_ImplSDL3_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
}

void
engine::renderUI()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void
engine::finalize()
{
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
engine_instance()
{
    static engine nano{};
    return nano;
}

} // namespace nano
