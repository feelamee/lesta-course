#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <nano/canvas.hpp>
#include <nano/vec.hpp>

#include <cstdint>
#include <filesystem>

namespace nano
{

class texture
{
public:
    texture() = default;
    texture(const canvas&);

    ~texture();

    int load(const canvas&);
    int load(const std::filesystem::path&, bool = false);
    vec2s size() const;
    std::size_t width() const;
    std::size_t height() const;
    std::uint32_t handle() const;
    void set_size(vec2s);
    void set_size(std::size_t, std::size_t);

    static bool exist(const texture& t);

private:
    std::uint32_t m_handle;
    vec2s m_size;
};

} // namespace nano

#endif // TEXTURE_HPP
