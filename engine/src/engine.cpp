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

#include "engine.hpp"
#include <errors.hpp>
#include <event.hpp>
#include <shader.hpp>
#include <transform.hpp>
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
    {
        OM_GL_CHECK();
        std::cerr << "Such program isn't exist" << std::endl;
        return;
    }
    OM_GL_CHECK();

    program = p_program;
    glUseProgram(program);
    OM_GL_CHECK();
}

static int
prepare_default_gl()
{
    const char* vertex_shader_src = R"(
                                      #version 320 es

                                      layout (location = 0) in vec2 p_pos;
                                      layout (location = 1) in vec3 p_color;

                                      out vec3 color;

                                      void main()
                                      {
                                          gl_Position = vec4(p_pos, 0, 1.0);
                                          color = p_color;
                                      }
                                    )";

    const char* fragment_shader_src = R"(
                                        #version 320 es
                                        precision mediump float;

                                        in vec3 color;

                                        out vec4 o_color;

                                        void main()
                                        {
                                            o_color = vec4(color, 1);
                                        }
                                      )";

    char compile_info[256];
    GLuint vertex_shader =
        create_shader(static_cast<shader_t>(GL_VERTEX_SHADER),
                      vertex_shader_src,
                      compile_info,
                      256);
    if (-1 == vertex_shader)
    {
        std::cerr << compile_info;
        return -1;
    }
    GLuint fragment_shader =
        create_shader(static_cast<shader_t>(GL_FRAGMENT_SHADER),
                      fragment_shader_src,
                      compile_info,
                      256);
    if (-1 == fragment_shader)
    {
        std::cerr << compile_info;
        return -1;
    }

    GLuint program = glCreateProgram();
    OM_GL_CHECK();
    assert(0 != program && "Failed to create GLES program");

    glAttachShader(program, vertex_shader);
    OM_GL_CHECK();
    glAttachShader(program, fragment_shader);
    OM_GL_CHECK();

    glLinkProgram(program);
    OM_GL_CHECK();
    GLint exit_code = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &exit_code);
    OM_GL_CHECK();
    if (GL_FALSE == exit_code)
    {
        GLint len = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        OM_GL_CHECK();
        std::string info;
        info.resize(len);
        glGetProgramInfoLog(program, len, nullptr, info.data());
        OM_GL_CHECK();
        std::cerr << info;
        glDeleteProgram(program);
        OM_GL_CHECK();
        return -1;
    }

    return program;
}

int
engine::initialize()
{

    sdl_init = raii_wrapper<int, void>(SDL_Init(SDL_INIT_EVERYTHING), SDL_Quit);
    ASSERT_SDL_ERROR(EXIT_SUCCESS == sdl_init.constructor_ret_v);

    // TODO: extract such values to config
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    constexpr size_t w = 720, h = 480;

    window = std::shared_ptr(SDL_CreateWindow("test", w, h, SDL_WINDOW_OPENGL),
                             SDL_DestroyWindow);
    ASSERT_SDL_ERROR(nullptr != window);

    // TODO: extract such values to config
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
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
    assert(real_major_version >= 3 && real_minor_version >= 2);

    assert(0 != gladLoadGLES2Loader(SDL_GL_GetProcAddress));

    program = prepare_default_gl();
    if (-1 == program)
        return EXIT_FAILURE;
    set_program(program);
    glViewport(0, 0, w, h);

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
    const int location = glGetUniformLocation(program, name.c_str());
    OM_GL_CHECK();
    if (location == -1)
    {
        if (!glIsProgram(program))
            std::cerr << program << " isn't a gl program" << std::endl;

        throw std::runtime_error("can't get uniform location: " + name);
    }

    glActiveTexture(GL_TEXTURE0);
    OM_GL_CHECK();

    glUniform1i(location, 0);
    OM_GL_CHECK();
}

void
engine::set_uniform(const std::string& name, const transform* p_transform)
{
    assert(p_transform != nullptr);
    if (GL_FALSE == glIsProgram(program))
        std::cerr << program << " isn't a gl program" << std::endl;

    const int location = glGetUniformLocation(program, name.c_str());
    OM_GL_CHECK();
    if (location == -1)
    {
        throw std::runtime_error("can't get uniform location: " + name);
    }

    glUniformMatrix3fv(location, 1, GL_FALSE, p_transform->data());
    OM_GL_CHECK();
}

} // namespace nano
