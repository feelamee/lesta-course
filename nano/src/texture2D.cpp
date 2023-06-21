#include <nano/texture2D.hpp>

#include <nano/error.hpp>
#include <nano/resource_loader.hpp>
#include <nano/utils.hpp>
#include <nano/vec.hpp>

#include <cstdlib>
#include <fstream>

#include <iostream>

namespace nano
{

texture2D::texture2D(const canvas& img)
{
    load(img);
}

texture2D::texture2D(texture2D&& other) noexcept
{
    std::swap(*this, other);
}

texture2D&
texture2D::operator=(texture2D&& other) noexcept
{
    std::swap(*this, other);
    return *this;
}

texture2D::~texture2D()
{
    if (exist(*this))
    {
        GL_CHECK(glDeleteTextures(1, &m_handle));
    }
}

vec2s
texture2D::size() const
{
    return m_size;
}

std::size_t
texture2D::width() const
{
    return m_size.x;
}

std::size_t
texture2D::height() const
{
    return m_size.y;
}

std::uint32_t
texture2D::handle() const
{
    return m_handle;
}

void
texture2D::size(std::size_t w, std::size_t h)
{
    m_size = { w, h };
}

void
texture2D::size(vec2s size)
{
    m_size = size;
}

int
texture2D::load(const canvas& image)
{
    remove(*this);

    GL_CHECK(glGenTextures(1, &m_handle));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_handle));

    size(image.width(), image.height());
    GL_ASSERT(glTexImage2D(GL_TEXTURE_2D,
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

int
texture2D::load(const std::filesystem::path& filename)
{
    canvas img;
    int err_code = image::load(filename, img);
    ASSERT_ERROR(err_code,
                 "Fail when loading canvas:    %s",
                 image::error2str(err_code).c_str());

    err_code = load(img);
    ASSERT_ERROR(err_code, "Failed loading texture from canvas");

    return EXIT_SUCCESS;
}

bool
texture2D::exist(const texture2D& t)
{
    GLboolean ret{ false };
    GL_CHECK(ret = glIsTexture(t.m_handle));
    return ret;
}

int
texture2D::bind(const texture2D& t)
{
    if (not exist(t))
    {
        LOG_DEBUG("Texture does not exist.\n");
        return EXIT_FAILURE;
    }
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, t.m_handle));
    return EXIT_SUCCESS;
}

void
texture2D::remove(texture2D& t)
{
    if (exist(t))
    {
        GL_CHECK(glDeleteTextures(1, &t.m_handle));
        t.m_handle = 0;
    }
}

std::size_t
texture2D::max_active()
{
    static GLint max{ 0 };
    GL_CHECK(glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max));
    return max < 0 ? 0 : max;
}

} // namespace nano
