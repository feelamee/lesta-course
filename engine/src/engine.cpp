#include <chrono>
#include <cstdlib>
using namespace std::literals::chrono_literals;

#include <assert.h>
#include <iostream>
#include <memory>
#include <thread>

// see ~/code/lesta-course/external/SDL/include/SDL3/SDL_stdinc.h line 795
#define SDL_FUNCTION_POINTER_IS_VOID_POINTER
#include <SDL3/SDL.h>
#include <glad/glad.h>

#include "engine.hpp"
#include <event.hpp>
#include <shader.hpp>
#include <vertex.hpp>

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

namespace nano
{

void
engine::set_program(uint p_program)
{
    if (!glIsProgram(p_program))
        return;

    program = p_program;

    glLinkProgram(program);
    GLint exit_code = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &exit_code);
    if (GL_FALSE == exit_code)
    {
        GLint len = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        std::string info;
        info.reserve(len);
        glGetShaderInfoLog(program, len, nullptr, info.data());
        std::cerr << info;
        glDeleteProgram(program);
        return;
    }
    glUseProgram(program);
}

static int
prepare_default_gl()
{
    const char* vertex_shader_src = R"(
                                      #version 320 es

                                      layout (location = 0) in vec3 position;

                                      void main()
                                      {
                                          gl_Position = vec4(position, 1.0);
                                      }
                                    )";

    const char* fragment_shader_src = R"(
                                        #version 320 es
                                        precision mediump float;

                                        out vec4 color;

                                        void main()
                                        {
                                            color = vec4(1, 0, 0, 1);
                                        }
                                      )";

    char compile_info[128];
    GLuint vertex_shader =
        create_shader(static_cast<shader_t>(GL_VERTEX_SHADER),
                      vertex_shader_src,
                      compile_info,
                      128);
    if (0 == vertex_shader)
    {
        std::cerr << compile_info;
        return 0;
    }
    GLuint fragment_shader =
        create_shader(static_cast<shader_t>(GL_FRAGMENT_SHADER),
                      fragment_shader_src,
                      compile_info,
                      128);
    if (0 == fragment_shader)
    {
        std::cerr << compile_info;
        return 0;
    }

    GLuint program = glCreateProgram();
    if (0 == program)
        return 0;

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}

int
engine::initialize()
{

    sdl_init = raii_wrapper<int, void>(
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS), SDL_Quit);
    ASSERT_SDL_ERROR(EXIT_SUCCESS == sdl_init.constructor_ret_v);

    // TODO: extract such values to config
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    constexpr size_t w = 720, h = 480;

    window = std::shared_ptr(SDL_CreateWindow("test", w, h, SDL_WINDOW_OPENGL),
                             SDL_DestroyWindow);
    ASSERT_SDL_ERROR(nullptr != window);

    // TODO: extract such values to config
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

    context = std::shared_ptr<void>(
        SDL_GL_CreateContext(window.get()),
        [](SDL_GLContext context)
        {
            ASSERT_SDL_ERROR(EXIT_SUCCESS == SDL_GL_DeleteContext(context));
            return EXIT_SUCCESS;
        });
    ASSERT_SDL_ERROR(nullptr != context);

    GLint real_major_version{}, real_minor_version{};
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &real_major_version);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &real_minor_version);
    // TODO: extract such values to config
    assert(real_major_version >= 2 && real_minor_version >= 0);

    assert(0 != gladLoadGLES2Loader(SDL_GL_GetProcAddress));

    program = prepare_default_gl();
    if (0 == program)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

void
engine::finalize()
{
}

int
engine::swap_buffers()
{
    ASSERT_SDL_ERROR(EXIT_SUCCESS == SDL_GL_SwapWindow(window.get()));
    glClear(GL_COLOR_BUFFER_BIT);
    return EXIT_SUCCESS;
}

iengine&
engine_instance()
{
    static engine nano{};
    iengine& inano = nano;
    return inano;
}

void
engine::set_uniform(const std::string& name, const texture* p_texture)
{
    assert(p_texture != nullptr);
    const int location = glGetUniformLocation(program, name.data());
    if (location == -1)
    {
        std::cerr << "can't get uniform location from shader\n";
        throw std::runtime_error("can't get uniform location");
    }

    uint texture_unit = 0;
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glUniform1i(location, static_cast<int>(0 + texture_unit));
}

} // namespace nano
