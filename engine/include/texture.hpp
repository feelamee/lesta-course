#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <canvas.hpp>
#include <cstdint>
#include <vec.hpp>

namespace nano
{

class texture
{
    texture() = default;
    texture(const texture&) = delete;

    int load(const canvas&);

private:
    std::uint32_t handle;
    vec2i size;
};

} // namespace nano

#endif // TEXTURE_HPP
