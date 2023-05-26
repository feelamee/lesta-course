#ifndef SHADER_HPP
#define SHADER_HPP

#include <filesystem>
#include <string>

namespace nano
{

class shader
{
public:
    enum class type
    {
        fragment = 0x8B30,
        vertex = 0x8B31,
    };

    shader() = default;
    ~shader();

    shader(const shader&) = delete;
    shader& operator=(const shader&) = delete;

    shader(shader&&);
    shader& operator=(shader&&);

    int load(type t, const std::filesystem::path& filename);

private:
    int compile(std::string& src, type t);
    static char log[1024];

    unsigned int handle{ 0 };
    int cur_texture{ -1 };
};

} // namespace nano

#endif // SHADER_HPP
