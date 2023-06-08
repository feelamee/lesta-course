#include <nano/texture.hpp>

#include <nano/error.hpp>
#include <nano/resource_loader.hpp>
#include <nano/utils.hpp>
#include <nano/vec.hpp>

#include <glad/glad.h>

#include <cstdlib>
#include <fstream>

namespace nano
{

texture2D::texture2D(const canvas& img)
{
    load(img);
}

texture2D::texture2D(texture2D&& other) noexcept
{
    m_handle = other.m_handle;
    m_size = other.m_size;
}

texture2D&
texture2D::operator=(texture2D&& other) noexcept
{
    m_handle = other.m_handle;
    m_size = other.m_size;
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

void
texture2D::set_size(std::size_t width, std::size_t height)
{
    m_size = { width, height };
}

void
texture2D::set_size(vec2s size)
{
    m_size = size;
}

int
texture2D::load(const canvas& image)
{
    remove(*this);

    GL_CHECK(glGenTextures(1, &m_handle));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_handle));

    set_size(image.width(), image.height());
    GL_ASSERT(glTexImage2D(GL_TEXTURE_2D,
                           0,
                           GL_RGB,
                           size().x,
                           size().y,
                           0,
                           GL_RGB,
                           GL_UNSIGNED_BYTE,
                           image.data()));

    // TODO: add methods for this
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

    // because glIsTexture return GL_FALSE while texture is not binded firstly
    glBindTexture(GL_TEXTURE_2D, m_handle);
    glBindTexture(GL_TEXTURE_2D, 0);

    return EXIT_SUCCESS;
}

int
texture2D::load(const std::filesystem::path& filename)
{
    std::ifstream file(filename);
    if (not file)
    {
        LOG_DEBUG("Failed opening file: " + path2str(filename));
        return EXIT_FAILURE;
    }

    canvas img;
    int err_code = ppm::load(file, img);
    if (EXIT_FAILURE == err_code)
    {
        LOG_DEBUG("Fail" + ppm::error2str(ppm::error()) +
                  " when loading canvas from: " + path2str(filename));
        return EXIT_FAILURE;
    }
    err_code = EXIT_FAILURE;

    err_code = load(img);
    if (EXIT_FAILURE == err_code)
    {
        LOG_DEBUG("Failed loading texture from canvas from file: " +
                  path2str(filename));
        return EXIT_FAILURE;
    }

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
        LOG_DEBUG("Texture does not exist.");
        return EXIT_FAILURE;
    }
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, t.m_handle));
    return EXIT_SUCCESS;
}

void
texture2D::remove(const texture2D& t)
{
    if (exist(t))
    {
        GL_CHECK(glDeleteTextures(1, &t.m_handle));
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
