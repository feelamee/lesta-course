#ifndef COLOR_HPP
#define COLOR_HPP

#include <cstdint>

#include <numeric>

namespace nano
{

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

namespace colors
{
inline static constexpr auto min = std::numeric_limits<color::channel_t>::min();
inline static constexpr auto max = std::numeric_limits<color::channel_t>::max();

inline constexpr color black{ min, min, min };
inline constexpr color white{ max, max, max };
inline constexpr color red{ max, min, min };
inline constexpr color green{ min, max, min };
inline constexpr color blue{ min, min, max };
} // namespace colors

} // namespace nano

#endif // COLOR_HPP
