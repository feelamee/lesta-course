#include <nano/engine.hpp>

#include <nano/error.hpp>
#include <nano/event.hpp>
#include <nano/shader.hpp>
#include <nano/transform2D.hpp>
#include <nano/vertex.hpp>

#define SDL_FUNCTION_POINTER_IS_VOID_POINTER
#include <SDL3/SDL.h>

#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

#include <glad/glad.h>

#include <cassert>
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

std::filesystem::path
engine::assets_path()
{
#ifdef __ANDROID__
    return "";
#else
    return "../assets";
#endif
}

int
engine::initialize(int init_flags)
{
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);
    flags = init_flags;

    impl = std::make_shared<impl_t>();
    int err_code = SDL_Init(init_flags);
    ASSERT_SDL_ERROR(EXIT_SUCCESS == err_code, EXIT_FAILURE);

    if (not(flag::video & flags))
    {
        return EXIT_SUCCESS;
    }

#ifdef __ANDROID__
    {
        const SDL_DisplayMode* display_mode{ nullptr };
        int display_count{ 0 };
        SDL_DisplayID* display_ids = SDL_GetDisplays(&display_count);
        ASSERT_SDL_ERROR(nullptr != display_ids, EXIT_FAILURE);
        int i{ 0 };
        SDL_DisplayID* id{ display_ids };
        while (nullptr == display_mode and i < display_count)
        {
            display_mode = SDL_GetCurrentDisplayMode(*id);
            ++i;
            ++id;
        }
        SDL_free(display_ids);

        ASSERT_SDL_ERROR(nullptr != display_mode, EXIT_FAILURE);
        window.size.x = display_mode->w;
        window.size.y = display_mode->h;
    }
#endif

    impl->window = SDL_CreateWindow(
        "test", window.size.x, window.size.y, SDL_WINDOW_OPENGL);
    ASSERT_SDL_ERROR(nullptr != impl->window, EXIT_FAILURE);

    // TODO: extract such values to config
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

#ifdef __WINDOWS__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#endif

    GLint real_major_version{}, real_minor_version{};
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &real_major_version);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &real_minor_version);

    // TODO: extract such values to config
    assert(real_major_version == 3 && real_minor_version == 2);

    impl->context = SDL_GL_CreateContext(impl->window);
    ASSERT_SDL_ERROR(nullptr != impl->context, EXIT_FAILURE);

    assert(0 != gladLoadGLES2Loader(SDL_GL_GetProcAddress));

    IMGUI_CHECKVERSION();
    ImGuiContext* imgui_ctx = ImGui::CreateContext();
    if (nullptr == imgui_ctx)
    {
        LOG_DEBUG("Failed while creating Dear ImGui context\n");
        return EXIT_FAILURE;
    }

    // TODO: implement this methods by yourself
    bool err = not ImGui_ImplSDL3_InitForOpenGL(impl->window, impl->context);
    if (err)
    {
        LOG_DEBUG("Failed while initializing SDL3 for OpenGL\n");
        return EXIT_FAILURE;
    }

    ImGui_ImplOpenGL3_Init("#version 300 es");
    return EXIT_SUCCESS;
}

void
engine::new_frame()
{
    if (not(flag::video & flags))
        return;

    // TODO: implement this methods by yourself
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
    // TODO: implement this methods by yourself
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

engine::~engine()
{
    if (m_instance.expired())
        return;

    if (not(flag::video & flags))
        return;

    // TODO: implement this methods by yourself
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

int
engine::swap_buffers()
{
    TEST_SDL_ERROR(EXIT_SUCCESS == SDL_GL_SwapWindow(impl->window));
    glClearColor(240. / 255, 244. / 255, 215. / 255, 1);
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
    return EXIT_SUCCESS;
}

std::weak_ptr<engine> engine::m_instance;

std::shared_ptr<engine>
engine::instance()
{
    if (not m_instance.expired())
    {
        return m_instance.lock();
    }
    auto&& ret = std::shared_ptr<engine>(new engine);
    m_instance = ret;
    return ret;
}

} // namespace nano
