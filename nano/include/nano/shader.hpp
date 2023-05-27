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
    static std::string type2str(type);

    shader();
    ~shader();

    shader(const shader&) = delete;
    shader& operator=(const shader&) = delete;

    shader(shader&&) noexcept;
    shader& operator=(shader);

    int load(type t, const std::filesystem::path& filename);

    int load(const std::filesystem::path& frag_fn,
             const std::filesystem::path& vert_fn);

    static int use(const shader&);

    static int link(const shader&);

    static int validate(const shader&);

    static bool exist(const shader&);

    static bool is_attached(type, const shader&);

    static unsigned int active();

    static void remove(shader&);

private:
    int compile(type t, const std::string& src);
    static char log[1024];
    const static std::size_t shaders_max_count{ 2 };

    unsigned int handle{ 0 };
    int texture{ -1 };
};

} // namespace nano

#endif // SHADER_HPP
