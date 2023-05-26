#ifndef GL_CHECK_HPP
#define GL_CHECK_HPP

#include <glad/glad.h>

#include <filesystem>
#include <iosfwd>
#include <string_view>

#ifdef NANO_DEBUG
#define GL_CHECK(expr)                                                         \
    {                                                                          \
        expr;                                                                  \
        gl_check(__FILE__, __LINE__, #expr);                                   \
    }
#else
#define GL_CHECK(expr) (expr)
#endif
namespace nano
{

extern std::ostream& err;

void gl_check(const std::filesystem::path& filename,
              unsigned int line,
              std::string_view expr);

} // namespace nano

#endif // GL_CHECK_HPP
