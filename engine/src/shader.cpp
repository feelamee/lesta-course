#include <glad/glad.h>

class shader
{
};

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
        glGetShaderInfoLog(shader, buf_size, nullptr, compile_info);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}
