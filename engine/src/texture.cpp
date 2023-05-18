#include "vec.hpp"
#include <errors.hpp>
#include <glad/glad.h>
#include <texture.hpp>

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

std::uint32_t
texture::get_handle() const
{
    return handle;
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
                 GL_RGB,
                 size().snd,
                 size().fst,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 &image(0, 0));
    OM_GL_CHECK();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    OM_GL_CHECK();

    return EXIT_SUCCESS;
}

} // namespace nano
