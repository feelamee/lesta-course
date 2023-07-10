#include <nano/error.hpp>

#include <SDL3/SDL_log.h>

#include <cstdlib>
#include <iostream>

namespace nano
{

void
log_debug(int line, const char* fn, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    SDL_LogMessageV(
        SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, fmt, args);
    va_end(args);
}

int
gl_check(const std::filesystem::path& file,
         unsigned int line,
         std::string_view expression)
{
    GLenum error_code = glGetError();
    if (GL_NO_ERROR == error_code)
    {
        return EXIT_SUCCESS;
    }

    do
    {
        std::string error = "Unknown error";
        std::string description = "No description";

        // Decode the error code
        switch (error_code)
        {
        case GL_INVALID_ENUM:
        {
            error = "GL_INVALID_ENUM";
            description = "An unacceptable value has been specified for an "
                          "enumerated argument.";
            break;
        }

        case GL_INVALID_VALUE:
        {
            error = "GL_INVALID_VALUE";
            description = "A numeric argument is out of range.";
            break;
        }

        case GL_INVALID_OPERATION:
        {
            error = "GL_INVALID_OPERATION";
            description = "The specified operation is not allowed in the "
                          "current state.";
            break;
        }

        case GL_OUT_OF_MEMORY:
        {
            error = "GL_OUT_OF_MEMORY";
            description =
                "There is not enough memory left to execute the command.";
            break;
        }

        case GL_INVALID_FRAMEBUFFER_OPERATION:
        {
            error = "GL_INVALID_FRAMEBUFFER_OPERATION";
            description = "The object bound to FRAMEBUFFER_BINDING is not "
                          "\"framebuffer complete\".";
            break;
        }
        }

        LOG_DEBUG("[ %s : %d ]:"
                  "\n    An internal OpenGL call failed"
                  "\n    Expression:\n        %s"
                  "\n    Error description:\n        %s\n",
                  file.string().c_str(),
                  line,
                  expression.data(),
                  description.c_str());

        error_code = glGetError();
    } while (false); // GL_NO_ERROR != error_code);

    return EXIT_FAILURE;
}

} // namespace nano
