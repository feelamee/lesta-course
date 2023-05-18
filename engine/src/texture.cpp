#include "vec.hpp"
#include <glad/glad.h>
#include <texture.hpp>

#define OM_GL_CHECK()                                                          \
    {                                                                          \
        const unsigned int err = glGetError();                                 \
        if (err != GL_NO_ERROR)                                                \
        {                                                                      \
            switch (err)                                                       \
            {                                                                  \
            case GL_INVALID_ENUM:                                              \
                std::cerr << "GL_INVALID_ENUM" << std::endl;                   \
                break;                                                         \
            case GL_INVALID_VALUE:                                             \
                std::cerr << "GL_INVALID_VALUE" << std::endl;                  \
                break;                                                         \
            case GL_INVALID_OPERATION:                                         \
                std::cerr << "GL_INVALID_OPERATION" << std::endl;              \
                break;                                                         \
            case GL_INVALID_FRAMEBUFFER_OPERATION:                             \
                std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl;  \
                break;                                                         \
            case GL_OUT_OF_MEMORY:                                             \
                std::cerr << "GL_OUT_OF_MEMORY" << std::endl;                  \
                break;                                                         \
            default:                                                           \
                std::cerr << "Unknown error" << std::endl;                     \
            }                                                                  \
            assert(false);                                                     \
        }                                                                      \
    }

namespace nano
{

texture::texture(const canvas& img)
{
    load(img);
}

texture::~texture()
{
    glDeleteTextures(1, &handle);
}

vec2i
texture::size() const
{
    return m_size;
}

void
texture::set_size(vec2i new_size)
{
    m_size = new_size;
}

int
texture::load(const canvas& image)
{
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);

    set_size(
        { static_cast<int>(image.width()), static_cast<int>(image.height()) });
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB8UI,
                 size().snd,
                 size().fst,
                 0,
                 GL_RGB_INTEGER,
                 GL_UNSIGNED_BYTE,
                 &image(0, 0));
    OM_GL_CHECK();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return EXIT_SUCCESS;
}

} // namespace nano
