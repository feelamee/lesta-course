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
#include <nano/error.hpp>
#include <nano/event.hpp>
#include <nano/shader.hpp>
#include <nano/transform.hpp>
#include <nano/vertex.hpp>

namespace nano
{

int
engine::initialize()
{
    sdl_init = raii_wrapper<int, void>(SDL_Init(SDL_INIT_EVERYTHING), SDL_Quit);
    ASSERT_SDL_ERROR(EXIT_SUCCESS == sdl_init.constructor_ret_v);

    // TODO: extract such values to config
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    constexpr size_t w = 720, h = 480;

    window = std::shared_ptr<SDL_Window>(
        SDL_CreateWindow("test", w, h, SDL_WINDOW_OPENGL), SDL_DestroyWindow);
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

    return EXIT_SUCCESS;
}

void
engine::finalize()
{
}

engine::~engine()
{
}

int
engine::swap_buffers()
{
    ASSERT_SDL_ERROR(EXIT_SUCCESS == SDL_GL_SwapWindow(window.get()));
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
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
    if (p_texture and not texture::exist(*p_texture))
    {
        LOG_DEBUG("Texture does not exist");
        return;
    }

    GLuint program = shader::active();
    if (not program)
    {
        LOG_DEBUG("Shader program does not exist");
        return;
    }

    int location{ -1 };
    GL_CHECK(location = glGetUniformLocation(program, name.c_str()));
    if (location == -1)
    {
        LOG_DEBUG("Failed to get uniform location: " + name);
        return;
    }

    GL_CHECK(glActiveTexture(GL_TEXTURE0));
    GL_CHECK(glUniform1i(location, 0));
}

void
engine::set_uniform(const std::string& name, const transform2D& p_transform)
{
    GLuint program = shader::active();
    if (not program)
    {
        LOG_DEBUG("Shader program does not exist");
        return;
    }

    int location{ 0 };
    GL_CHECK(location = glGetUniformLocation(program, name.c_str()));
    if (location == -1)
    {
        LOG_DEBUG("Cant't get uniform location: " + name);
        return;
    }

    GL_CHECK(glUniformMatrix3fv(location, 1, GL_FALSE, p_transform.data()));
}

} // namespace nano
