#include <cstdlib>
#include <texture.hpp>

#include <glad/glad.h>

#include <glcheck.hpp>
#include <vec.hpp>

namespace nano
{

texture::texture(const canvas& img)
{
    load(img);
}

texture::~texture()
{
    GLboolean is_exist{ false };
    GL_CHECK(is_exist = glIsTexture(handle));
    if (GL_TRUE == is_exist)
        GL_CHECK(glDeleteTextures(1, &handle));
}

vec2s
texture::size() const
{
    return m_size;
}

std::size_t
texture::width() const
{
    return m_size.x;
}

std::size_t
texture::height() const
{
    return m_size.y;
}

std::uint32_t
texture::get_handle() const
{
    return handle;
}

void
texture::set_size(std::size_t width, std::size_t height)
{
    m_size = { width, height };
}

void
texture::set_size(vec2s size)
{
    m_size = size;
}

int
texture::load(const canvas& image)
{
    GL_CHECK(glGenTextures(1, &handle));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, handle));

    set_size(image.width(), image.height());
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D,
                          0,
                          GL_RGB,
                          size().x,
                          size().y,
                          0,
                          GL_RGB,
                          GL_UNSIGNED_BYTE,
                          image.data()));

    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

    GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));

    return EXIT_SUCCESS;
}

} // namespace nano
