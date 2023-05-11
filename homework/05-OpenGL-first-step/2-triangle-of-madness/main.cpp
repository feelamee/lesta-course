// see ~/code/lesta-course/external/SDL/include/SDL3/SDL_stdinc.h line 795
#define SDL_FUNCTION_POINTER_IS_VOID_POINTER
#include <SDL3/SDL.h>

#include <assert.h>
#include <glad/glad.h>

#include "raii_wrapper.hpp"

#include <array>
#include <cstdlib>
#include <filesystem>
#include <fstream>
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

GLuint
create_shader(GLenum, const char*, char*, GLuint);

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

    // clang-format off
    constexpr std::array<GLfloat, 9> vertices{
        -0.5f, -0.5f, 0.f,
         0.5f, -0.5f, 0.f,
         0.0f,  0.5f, 0.f,
    };
    // clang-format on

    GLuint buf;
    glGenBuffers(1, &buf);
    glBindBuffer(GL_ARRAY_BUFFER, buf);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(decltype(vertices)::value_type) * vertices.size(),
                 vertices.data(),
                 GL_STATIC_DRAW);

    auto deleter = [](auto ptr)
    {
        if (ptr)
            delete[] ptr;
    };
    std::shared_ptr<char> vertex_shader_src, fragment_shader_src;
    {
        const char* vertex_shader_src_path =
            "../homework/05-OpenGL-first-step/2-triangle-of-madness/"
            "vertex-shader.vert";
        const char* fragment_shader_src_path =
            "../homework/05-OpenGL-first-step/2-triangle-of-madness/"
            "fragment-shader.frag";
        std::fstream vertex_shader_stream(vertex_shader_src_path, std::ios::in);
        std::fstream fragment_shader_stream(fragment_shader_src_path,
                                            std::ios::in);
        size_t vertex_shader_src_size =
                   std::filesystem::file_size(vertex_shader_src_path),
               fragment_shader_src_size =
                   std::filesystem::file_size(fragment_shader_src_path);

        vertex_shader_src =
            std::shared_ptr<char>(new char[vertex_shader_src_size]{}, deleter);
        fragment_shader_src = std::shared_ptr<char>(
            new char[fragment_shader_src_size]{}, deleter);

        vertex_shader_stream.read(vertex_shader_src.get(),
                                  vertex_shader_src_size);
        fragment_shader_stream.read(fragment_shader_src.get(),
                                    fragment_shader_src_size);
    }

    char compile_info[128];
    GLuint vertex_shader = create_shader(
        GL_VERTEX_SHADER, vertex_shader_src.get(), compile_info, 128);
    if (0 == vertex_shader)
    {
        std::cerr << compile_info;
        return EXIT_FAILURE;
    }
    GLuint fragment_shader = create_shader(
        GL_FRAGMENT_SHADER, fragment_shader_src.get(), compile_info, 128);
    if (0 == fragment_shader)
    {
        std::cerr << compile_info;
        return EXIT_FAILURE;
    }

    GLuint program = glCreateProgram();
    assert(0 != program && "Failed to create GLES program");

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    glLinkProgram(program);
    GLint exit_code = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &exit_code);
    if (GL_FALSE == exit_code)
    {
        GLint len = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        std::string compile_info;
        compile_info.reserve(len);
        glGetShaderInfoLog(program, len, nullptr, compile_info.data());
        std::cerr << compile_info;
        glDeleteShader(program);
        return 0;
    }
    glValidateProgram(program);
    exit_code = 0;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &exit_code);
    if (GL_FALSE == exit_code)
    {
        GLint len = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        std::string compile_info;
        compile_info.reserve(len);
        glGetShaderInfoLog(program, len, nullptr, compile_info.data());
        std::cerr << compile_info;
        glDeleteShader(program);
        return 0;
    }

    glUseProgram(program);
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    SDL_Event ev;
    while (true)
    {
        while (SDL_PollEvent(&ev))
        {
            switch (ev.type)
            {
            case SDL_EVENT_QUIT:
                return EXIT_SUCCESS;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 1, 1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        TEST_SDL_ERROR(EXIT_SUCCESS == SDL_GL_SwapWindow(window.get()));
    }

    return EXIT_SUCCESS;
}

GLuint
create_shader(GLenum type, const char* src, char* compile_info, GLuint buf_size)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint exit_code{};
    glGetShaderiv(shader, GL_COMPILE_STATUS, &exit_code);
    if (GL_FALSE == exit_code)
    {
        // GLint len = 0;
        // glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        // compile_info.reserve(len);
        glGetShaderInfoLog(shader, buf_size, nullptr, compile_info);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}
