#include <nano/texture.hpp>

#include <nano/error.hpp>
#include <nano/image_loader.hpp>
#include <nano/utils.hpp>
#include <nano/vec.hpp>

#include <glad/glad.h>

#include <cstdlib>
#include <fstream>

namespace nano
{

texture::texture(const canvas& img)
{
    load(img);
}

texture::~texture()
{
    if (exist(*this))
    {
        GL_CHECK(glDeleteTextures(1, &m_handle));
    }
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
texture::handle() const
{
    return m_handle;
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
    GL_CHECK(glGenTextures(1, &m_handle));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_handle));

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

    GL_CHECK(glActiveTexture(GL_TEXTURE0));

    return EXIT_SUCCESS;
}

int
texture::load(const std::filesystem::path& filename, bool transpose)
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

    if (transpose)
    {
        img.transpose();
    }

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
texture::exist(const texture& t)
{
    GLboolean ret{ false };
    GL_CHECK(ret = glIsTexture(t.m_handle));
    return ret;
}

} // namespace nano
