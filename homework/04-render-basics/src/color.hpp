#include <cstdint>

class color
{
    float r;
    float g;
    float b;
    float a;

    color(float p_r, float p_g, float p_b, float p_a = 1)
        : r(p_r), g(p_g), b(p_b), a(p_a)
    {
    }

    bool operator==(const color& rhs)
    {
        return (r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a);
    }

    static color hex(std::uint32_t p_hex);
};