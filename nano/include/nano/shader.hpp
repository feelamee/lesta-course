#ifndef SHADER_HPP
#define SHADER_HPP

#include <nano/texture2D.hpp>
#include <nano/transform2D.hpp>

#include <filesystem>
#include <string>
#include <unordered_map>

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
    shader& operator=(shader&&) noexcept;

    int load(type t, const std::filesystem::path& filename);

    int load(const std::filesystem::path& frag_fn,
             const std::filesystem::path& vert_fn);

    int load_from_src(type t, const std::string& sources);

    int load_from_src(const std::string& frag_src, const std::string& vert_src);

    int uniform(const std::string&, const texture2D&);
    int uniform(const std::string&, const transform2D&);

    using location_t = int;
    location_t uniform_location(const std::string&) const;

    void bind_textures() const;

    static int use(const shader&);
    static int link(const shader&);
    static int validate(const shader&);
    static bool exist(const shader&);
    static bool is_attached(type, const shader&);
    static int active();
    static void remove(shader&);

    struct state_guard;

private:
    int compile(type t, const std::string& src);

    constexpr static void clear_log_buf();
    static char log[1024];

    constexpr static std::size_t max_count{ 2 };

    std::unordered_map<location_t, const texture2D&> textures;
    mutable std::unordered_map<std::string, location_t> uniforms;
    unsigned int handle{ 0 };
};

namespace shaders
{
namespace vert
{
std::string transform_src(const std::string& transform_arg_name);
} // namespace vert

namespace frag
{
std::string texture_src(const std::string& texture_arg_name);
} // namespace frag

int transform_texture(shader& result,
                      const texture2D& tex = {},
                      const transform2D& tr = {});

} // namespace shaders

} // namespace nano

#endif // SHADER_HPP
