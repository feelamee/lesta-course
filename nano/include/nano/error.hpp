#ifndef ERROR_HPP
#define ERROR_HPP

#include <glad/glad.h>

#include <filesystem>
#include <iosfwd>
#include <string_view>

namespace nano
{
extern std::ostream& err_os;
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

#define LOG_DEBUG(message)                                                     \
    {                                                                          \
        nano::err_os << "[DEBUG: " << __FILE__ << ":" << __LINE__              \
                     << "]:\n    " << (message) << '\n'                        \
                     << std::endl;                                             \
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
            LOG_DEBUG(SDL_GetError());                                         \
            return EXIT_FAILURE;                                               \
        }                                                                      \
    }

#define TEST_SDL_ERROR(expr)                                                   \
    {                                                                          \
        if (!(expr))                                                           \
        {                                                                      \
            LOG_DEBUG(SDL_GetError());                                         \
        }                                                                      \
    }

namespace nano
{

int gl_check(const std::filesystem::path& filename,
             unsigned int line,
             std::string_view expr);

} // namespace nano

#endif // ERROR_HPP
