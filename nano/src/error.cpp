#include <nano/error.hpp>

#include <cstdlib>
#include <iostream>

namespace nano
{

FILE* err_os = stderr;

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

#ifndef __ANDROID__
        case GL_STACK_OVERFLOW:
        {
            error = "GL_STACK_OVERFLOW";
            description = "This command would cause a stack overflow.";
            break;
        }

        case GL_STACK_UNDERFLOW:
        {
            error = "GL_STACK_UNDERFLOW";
            description = "This command would cause a stack underflow.";
            break;
        }
#endif

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

        fprintf(err_os,
                "[DEBUG: %s : %d ]:"
                "\n    An internal OpenGL call failed"
                "\n    Expression:\n        %s"
                "\n    Error description:\n        %s\n",
                file.string().c_str(),
                line,
                expression.data(),
                description.c_str());

        error_code = glGetError();
    } while (GL_NO_ERROR != error_code);

    return EXIT_FAILURE;
}

} // namespace nano
