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
#include <string>

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
    return "./assets";
#endif
}

void
engine::start()
{
    m_is_running = true;
}

void
engine::stop()
{
    m_is_running = false;
}

bool
engine::is_running()
{
    return m_is_running;
}

static int
android_toast(std::string msg, int dur, int gravity, int offx, int offy)
{
#ifdef __ANDROID__
    int err_code = SDL_AndroidShowToast(msg.c_str(), dur, gravity, offx, offy);
    ASSERT_SDL_ERROR(EXIT_SUCCESS == err_code, EXIT_FAILURE);
    return EXIT_SUCCESS;
#endif

    return EXIT_FAILURE;
}

static int
display_mode(const SDL_DisplayMode*& dm)
{
    int display_count{ 0 };
    SDL_DisplayID* display_ids = SDL_GetDisplays(&display_count);
    ASSERT_SDL_ERROR(nullptr != display_ids, EXIT_FAILURE);
    int i{ 0 };
    SDL_DisplayID* id{ display_ids };
    while (nullptr == dm and i < display_count)
    {
        dm = SDL_GetCurrentDisplayMode(*id);
        ++i;
        ++id;
    }
    SDL_free(display_ids);

    ASSERT_SDL_ERROR(nullptr != dm, EXIT_FAILURE);
    return EXIT_SUCCESS;
}

int
engine::initialize(int init_flags)
{
//    android_toast("Initialization begin", 1, 0, 0, 0);
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);
    flags = init_flags;

    impl = std::make_shared<impl_t>();
    int err_code = SDL_Init(init_flags);
    ASSERT_SDL_ERROR(EXIT_SUCCESS == err_code, EXIT_FAILURE);

    if (not(flag::video & flags))
    {
        return EXIT_SUCCESS;
    }

    const SDL_DisplayMode* dm{ nullptr };
    err_code = display_mode(dm);
    ASSERT_ERROR(err_code, "Failed to get display mode");
#ifdef __ANDROID__
    window.size.x = dm->w;
    window.size.y = dm->h;
#else
    // because taskbar occupy 10%???
    window.size.x = dm->h / 2. * 0.9;
    window.size.y = dm->h * 0.9;
#endif

//    android_toast("Window creating", 1, 0, 0, 0);
    impl->window = SDL_CreateWindow(
        "test", window.size.x, window.size.y, SDL_WINDOW_OPENGL);
    ASSERT_SDL_ERROR(nullptr != impl->window, EXIT_FAILURE);

    //----------------------------------------
    // OpenGL    GLSL      GLSL
    // version   version   string
    //----------------------------------------
    //  2.0       110       "#version 110"
    //  2.1       120       "#version 120"
    //  3.0       130       "#version 130"
    //  3.1       140       "#version 140"
    //  3.2       150       "#version 150"
    //  3.3       330       "#version 330 core"
    //  4.0       400       "#version 400 core"
    //  4.1       410       "#version 410 core"
    //  4.2       420       "#version 410 core"
    //  4.3       430       "#version 430 core"
    //  ES 2.0    100       "#version 100"      = WebGL 1.0
    //  ES 3.0    300       "#version 300 es"   = WebGL 2.0
    //----------------------------------------
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

#ifdef __WINDOWS__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#endif

    GLint real_major_version{}, real_minor_version{};
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &real_major_version);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &real_minor_version);

//    android_toast("OpenGLES Version " + std::to_string(real_major_version) +
//                      "." + std::to_string(real_minor_version),
//                  1,
//                  0,
//                  0,
//                  0);
    assert(real_major_version >= 2 and real_minor_version >= 0);

//    android_toast("OpenGLES context creating", 1, 0, 0, 0);
    impl->context = SDL_GL_CreateContext(impl->window);
    ASSERT_SDL_ERROR(nullptr != impl->context, EXIT_FAILURE);

//    android_toast("gladLoadGLES2Loader calling", 1, 0, 0, 0);
    err_code = gladLoadGLES2Loader(SDL_GL_GetProcAddress);
    assert(0 != err_code);

//    android_toast("Dear ImGui context creating", 1, 0, 0, 0);
    IMGUI_CHECKVERSION();
    ImGuiContext* imgui_ctx = ImGui::CreateContext();
    if (nullptr == imgui_ctx)
    {
        LOG_DEBUG("Failed while creating Dear ImGui context\n");
        return EXIT_FAILURE;
    }

//    android_toast("Dear ImGui initialization SDL3", 1, 0, 0, 0);
    bool err = not ImGui_ImplSDL3_InitForOpenGL(impl->window, impl->context);
    if (err)
    {
        LOG_DEBUG("Failed while initializing SDL3 for OpenGL\n");
        return EXIT_FAILURE;
    }

//    android_toast("Dear ImGui initialization OpenGLES", 1, 0, 0, 0);
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

engine::~engine()
{
    if (m_instance.expired())
        return;

    if (not(flag::video & flags))
        return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

int
engine::render()
{
    if (not(flag::video & flags))
    {
        return EXIT_SUCCESS;
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
