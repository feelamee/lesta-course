#ifndef ERROR_HPP
#define ERROR_HPP

#include <filesystem>
#include <iosfwd>
#include <string_view>

#include <glad/glad.h>

namespace nano
{
extern FILE* err_os;
}

#ifdef NANO_DEBUG
#define GL_CHECK(expr)                                                         \
    {                                                                          \
        expr;                                                                  \
        gl_check(__FILE__, __LINE__, #expr);                                   \
    }

#define GL_ASSERT(expr)                                                        \
    {                                                                          \
        expr;                                                                  \
        if (EXIT_FAILURE == gl_check(__FILE__, __LINE__, #expr))               \
        {                                                                      \
            return EXIT_FAILURE;                                               \
        }                                                                      \
    }

#define LOG_DEBUG(...)                                                         \
    {                                                                          \
        fprintf(nano::err_os, "[DEBUG: %s: %d]:\n    ", __FILE__, __LINE__);   \
        fprintf(nano::err_os, __VA_ARGS__);                                    \
    }

#define ASSERT_ERROR(err, ...)                                                 \
    {                                                                          \
        if (EXIT_FAILURE == (err))                                             \
        {                                                                      \
            LOG_DEBUG(__VA_ARGS__);                                            \
            return EXIT_FAILURE;                                               \
        }                                                                      \
        (err) = EXIT_FAILURE;                                                  \
    }

#else
#define GL_CHECK(expr) (expr)
#define GL_ASSERT(expr) (expr)
#define LOG_DEBUG(message)
#endif

#define ASSERT_SDL_ERROR(expr)                                                 \
    {                                                                          \
        if (!(expr))                                                           \
        {                                                                      \
            LOG_DEBUG("%s", SDL_GetError());                                   \
            return EXIT_FAILURE;                                               \
        }                                                                      \
    }

#define TEST_SDL_ERROR(expr)                                                   \
    {                                                                          \
        if (!(expr))                                                           \
        {                                                                      \
            LOG_DEBUG("%s", SDL_GetError());                                   \
        }                                                                      \
    }

namespace nano
{

int gl_check(const std::filesystem::path& filename,
             unsigned int line,
             std::string_view expr);

} // namespace nano

#endif // ERROR_HPP
