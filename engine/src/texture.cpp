#include <glad/glad.h>
#include <texture.hpp>

namespace nano
{

int
texture::load(const canvas& image)
{
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);

    GLsizei width = static_cast<GLsizei>(size.fst);
    GLsizei height = static_cast<GLsizei>(size.snd);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB8UI,
                 width,
                 height,
                 0,
                 GL_RGB8UI,
                 GL_RGB_INTEGER,
                 &image(0, 0));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return EXIT_SUCCESS;
}

} // namespace nano
