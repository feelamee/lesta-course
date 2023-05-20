#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <canvas.hpp>
#include <cstdint>
#include <vec.hpp>

namespace nano
{

class texture
{
public:
    texture() = default;
    texture(const canvas&);

    int load(const canvas&);
    vec2s size() const;
    std::size_t width() const;
    std::size_t height() const;
    std::uint32_t get_handle() const;
    void set_size(vec2s);
    void set_size(std::size_t, std::size_t);

    ~texture();

private:
    std::uint32_t handle;
    vec2s m_size;
};

} // namespace nano

#endif // TEXTURE_HPP
