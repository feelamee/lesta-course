#ifndef COLOR_HPP
#define COLOR_HPP

#include <cstdint>
#include <numeric>

; // trick to disable incorrect warning in clangd
#pragma pack(push, 1)
struct color
{
    using channel_t = std::uint8_t;
    channel_t r;
    channel_t g;
    channel_t b;

    friend bool
    operator==(color lhs, color rhs)
    {
        // clang-format off
        return lhs.r == rhs.r &&
               lhs.g == rhs.g &&
               lhs.b == rhs.b;
        // clang-format on
    }
};
#pragma pack(pop)

static_assert(sizeof(color) == 3);

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

#endif // COLOR_HPP
