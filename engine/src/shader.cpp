#include <cstdlib>
#include <shader.hpp>

#include <glad/glad.h>

namespace nano
{

shader::~shader()
{
}

shader::shader(shader&& other)
{
}
shader&
shader::operator=(shader&& other)
{
    return *this;
}

int
shader::load(type t, const std::filesystem::path& filename)
{
    return EXIT_FAILURE;
}

int
shader::compile(std::string& src, type t)
{
    return EXIT_FAILURE;
}

} // namespace nano
