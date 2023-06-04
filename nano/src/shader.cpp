#include "nano/texture.hpp"
#include <nano/shader.hpp>

#include <nano/error.hpp>
#include <nano/utils.hpp>

#include <glad/glad.h>

#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <ranges>
#include <system_error>

namespace nano
{

struct shader::state_guard
{
    state_guard(const shader& s)
    {
        cur_handle = s.handle;
        prev_handle = shader::active();
        if (prev_handle < 0)
            prev_handle = 0;

        if (prev_handle != cur_handle)
        {
            GL_CHECK(glUseProgram(cur_handle));
        }
    }

    ~state_guard()
    {
        if (prev_handle != cur_handle)
        {
            GL_CHECK(glUseProgram(prev_handle));
        }
    }

    std::uint32_t prev_handle{ 0 };
    std::uint32_t cur_handle{ 0 };
};

char shader::log[1024]{ 0 };

shader::shader()
{
    GL_CHECK(handle = glCreateProgram());
}

shader::~shader()
{
    remove(*this);
}

shader::shader(shader&& other) noexcept
{
    handle = other.handle;
}

shader&
shader::operator=(shader&& other) noexcept
{
    handle = other.handle;
    return *this;
}

constexpr void
shader::clear_log_buf()
{
    std::fill_n(log, sizeof(log), 0);
}

constexpr std::string
shader::type2str(type t)
{
    switch (static_cast<GLenum>(t))
    {
    case GL_FRAGMENT_SHADER:
        return "FRAGMENT_SHADER";

    case GL_VERTEX_SHADER:
        return "VERTEX_SHADER";

    default:
        return "UNKNOWN_SHADER_TYPE";
    }
}

// NOTE: if shader of such type is already attached to program, then
// EXIT_FAILURE will return. Call shader::remove before
int
shader::load(type t, const std::filesystem::path& filename)
{
    if (is_attached(t, *this))
    {
        LOG_DEBUG("Shader of such type is already attached: " + type2str(t) +
                  ". Call shader::remove before");
        return EXIT_FAILURE;
    }
    if (not exist(*this))
    {
        GL_CHECK(handle = glCreateProgram());
    }

    std::string src;
    int err_code = nano::extract_file(src, filename);
    if (EXIT_FAILURE == err_code)
    {
        LOG_DEBUG("Fail when loading file: " + path2str(filename));
        return EXIT_FAILURE;
    }
    err_code = EXIT_FAILURE;

    err_code = compile(t, src);
    if (EXIT_FAILURE == err_code)
    {
        LOG_DEBUG("Fail when compiling shader source from: " +
                  path2str(filename));
    }

    return err_code;
}

int
shader::load(const std::filesystem::path& frag_filename,
             const std::filesystem::path& vert_filename)
{
    int err_code = load(type::fragment, frag_filename);
    if (EXIT_FAILURE == err_code)
    {
        LOG_DEBUG("Fail when loading fragment shader from: " +
                  path2str(frag_filename));
        return EXIT_FAILURE;
    }
    err_code = EXIT_FAILURE;

    err_code = load(type::vertex, vert_filename);
    if (EXIT_FAILURE == err_code)
    {
        LOG_DEBUG("Fail when loading vertex shader from: " +
                  path2str(vert_filename));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int
shader::uniform(const std::string& name, const texture2D& tex)
{
    if (not texture2D::exist(tex))
    {
        LOG_DEBUG("Texture does not exist.");
        return EXIT_FAILURE;
    }

    int location = uniform_location(name);
    if (location < 0)
    {
        LOG_DEBUG("Failed getting location of uniform: " + name);
        return EXIT_FAILURE;
    }

    auto it = textures.find(location);
    if (it == textures.end())
    {
        if (textures.size() + 1 >= texture2D::max_active())
        {
            LOG_DEBUG("Failed because all textures units are already used");
            return EXIT_FAILURE;
        }
    }
    else
    {
        textures.erase(it);
    }
    auto [_, is_success] = textures.emplace(location, tex);

    return is_success ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
shader::uniform(const std::string& name, const transform2D& tr)
{
    state_guard program_guard(*this);
    int location = uniform_location(name);
    if (location < 0)
    {
        LOG_DEBUG("Failed getting location of uniform: " + name);
        return EXIT_FAILURE;
    }

    GL_CHECK(glUniformMatrix3fv(location, 1, GL_FALSE, tr.data()));
    return EXIT_SUCCESS;
}

shader::location_t
shader::uniform_location(const std::string& name)
{
    location_t location{ -1 };
    auto it = uniforms.find(name);
    if (it == uniforms.end())
    {
        GL_CHECK(location = glGetUniformLocation(handle, name.c_str()));
        if (location >= 0)
        {
            uniforms[name] = location;
        }
    }
    else
    {
        location = it->second;
    }

    return location;
}

void
shader::bind_textures() const
{
    auto it = textures.begin();
    for (GLsizei i{ 0 }; i < textures.size(); ++i, ++it)
    {
        // TODO: create method texture2D::active()
        GL_CHECK(glActiveTexture(GL_TEXTURE0 + i));
        texture2D::bind(it->second);
        GL_CHECK(glUniform1i(it->first, i));
    }
    GL_CHECK(glActiveTexture(GL_TEXTURE0));
}

int
shader::compile(type t, const std::string& src)
{
    GLuint shader_handle{ 0 };
    GL_CHECK(shader_handle = glCreateShader(static_cast<GLenum>(t)));
    if (0 == shader_handle)
    {
        LOG_DEBUG("Failed creating of shader handle.");
        return EXIT_FAILURE;
    }

    const char* const src_raw = src.c_str();
    int src_size = src.size();
    GL_CHECK(glShaderSource(shader_handle, 1, &src_raw, &src_size));

    GL_CHECK(glCompileShader(shader_handle));

    GLint compile_status{ 0 };
    GL_CHECK(glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &compile_status))
    if (GL_FALSE == compile_status)
    {
        clear_log_buf();
        glGetShaderInfoLog(shader_handle, sizeof(log), nullptr, log);
        LOG_DEBUG(log);
        GL_CHECK(glDeleteShader(shader_handle));
        return EXIT_FAILURE;
    }
    GL_CHECK(glAttachShader(handle, shader_handle));
    GL_CHECK(glDeleteShader(shader_handle));

    return EXIT_SUCCESS;
}

void
shader::remove(shader& p)
{
    if (exist(p))
    {
        GL_CHECK(glDeleteProgram(p.handle));
    }
}

bool
shader::exist(const shader& p)
{
    GLboolean ret{ false };
    GL_CHECK(ret = glIsProgram(p.handle));
    return ret;
}

int
shader::use(const shader& p)
{
    if (not exist(p))
    {
        LOG_DEBUG("Program does not exist");
        return EXIT_FAILURE;
    }

    GL_CHECK(glUseProgram(p.handle));
    p.bind_textures();

    return EXIT_SUCCESS;
}

int
shader::link(const shader& p)
{
    if (not exist(p))
    {
        LOG_DEBUG("Program does not exist");
        return EXIT_FAILURE;
    }

    GL_CHECK(glLinkProgram(p.handle));
    GLint link_status{ 0 };
    GL_CHECK(glGetProgramiv(p.handle, GL_LINK_STATUS, &link_status))
    if (GL_FALSE == link_status)
    {
        clear_log_buf();
        glGetProgramInfoLog(p.handle, sizeof(log), nullptr, log);
        LOG_DEBUG(log);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int
shader::validate(const shader& p)
{
    if (not exist(p))
    {
        LOG_DEBUG("Program does not exist");
        return EXIT_FAILURE;
    }

    GL_CHECK(glValidateProgram(p.handle));
    GLint validate_status{ 0 };
    GL_CHECK(glGetProgramiv(p.handle, GL_VALIDATE_STATUS, &validate_status))
    if (GL_FALSE == validate_status)
    {
        std::memset(log, 0, sizeof(log));
        glGetProgramInfoLog(p.handle, sizeof(log), nullptr, log);
        LOG_DEBUG(log);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

bool
shader::is_attached(type t, const shader& p)
{
    if (not exist(p))
    {
        return false;
    }
    GLuint shaders[shaders_max_count];
    GLint shader_t{ 0 };
    GLsizei count{ 0 };

    GL_CHECK(
        glGetAttachedShaders(p.handle, shaders_max_count, &count, shaders));

    for (GLsizei i{ 0 }; i < count; ++i)
    {
        GL_CHECK(glGetShaderiv(shaders[i], GL_SHADER_TYPE, &shader_t));
        if (static_cast<GLint>(t) == shader_t)
            return true;

        shader_t = 0;
    }
    return false;
}

unsigned int
shader::active()
{
    GLint cur{ 0 };
    GL_CHECK(glGetIntegerv(GL_CURRENT_PROGRAM, &cur));
    return cur;
}

} // namespace nano
