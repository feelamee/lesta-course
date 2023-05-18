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
    vec2i size() const;
    void set_size(vec2i);

    ~texture();

private:
    std::uint32_t handle;
    vec2i m_size;
};

} // namespace nano

#endif // TEXTURE_HPP
