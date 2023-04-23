#include "color.hpp"

color color::hex(std::uint32_t p_hex)
{
    float a = (p_hex & 0xFF) / 255.0f;
    p_hex >>= 8;
    float b = (p_hex & 0xFF) / 255.0f;
    p_hex >>= 8;
    float g = (p_hex & 0xFF) / 255.0f;
    p_hex >>= 8;
    float r = (p_hex & 0xFF) / 255.0f;

    return color(r, g, b, a);
}