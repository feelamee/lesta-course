#ifndef COLOR_HPP
#define COLOR_HPP

#include <cstdint>
#include <cstring>
#include <numeric>

namespace nano
{

; // trick to disable incorrect warning in clangd
#pragma pack(push, 1)
struct color
{
    using channel_t = std::uint8_t;
    channel_t r{ 0 };
    channel_t g{ 0 };
    channel_t b{ 0 };

    friend bool
    operator==(color lhs, color rhs)
    {
        // clang-format off
        return lhs.r == rhs.r &&
               lhs.g == rhs.g &&
               lhs.b == rhs.b;
        // clang-format on
    }

    static std::uint32_t
    hex(color c)
    {
        std::uint32_t ret{ 0 };
        ret = (int)c.r << 0;
        ret |= (int)c.g << 8;
        ret |= (int)c.b << 16;
        ret |= 255 << 24;

        return ret;
    }

    static color
    hex(std::uint32_t p_hex)
    {
        color ret{};
        ret.r = (p_hex >> 16) & 0xFF;
        ret.g = (p_hex >> 8) & 0xFF;
        ret.b = (p_hex >> 0) & 0xFF;
        return ret;
    }
};
#pragma pack(pop)

static_assert(sizeof(color) == 3,
              "Color is packed structure with 3 field 1 byte each");
static_assert(alignof(color) == 1);

namespace colors
{
static constexpr auto min = std::numeric_limits<color::channel_t>::min();
static constexpr auto max = std::numeric_limits<color::channel_t>::max();

constexpr color black{ min, min, min };
constexpr color white{ max, max, max };
constexpr color red{ max, min, min };
constexpr color green{ min, max, min };
constexpr color blue{ min, min, max };
} // namespace colors

} // namespace nano

#endif // COLOR_HPP
