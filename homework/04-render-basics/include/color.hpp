#ifndef COLOR_HPP
#define COLOR_HPP

#include <concepts>
#include <cstdint>
#include <limits>
#include <numeric>
#include <ostream>

template <typename compT = uint8_t>
    requires std::is_same_v<uint8_t, compT> or std::is_same_v<uint16_t, compT>
using color_channel_t = compT;

; // trick to disable incorrect warning in clangd
#pragma pack(push, 1)
struct color
{
    union
    {
        struct
        {
            color_channel_t<> r = 0;
            color_channel_t<> g = 0;
            color_channel_t<> b = 0;
        };
        char components[3 * sizeof(color_channel_t<>)];
    };

    friend bool
    operator==(const color& l, const color& r)
    {
        return l.r == r.r && l.g == r.g && l.b == r.b;
    }

    friend std::ostream&
    operator<<(std::ostream& os, color color_)
    {
        auto tmp = static_cast<color>(color_);
        return os << "[" << tmp.r << " " << tmp.g << " " << tmp.b << "]";
    }
};
#pragma pack(pop)

namespace colors
{
constexpr auto min = std::numeric_limits<color_channel_t<>>::min();
constexpr auto max = std::numeric_limits<color_channel_t<>>::max();

constexpr color black{ min, min, min };
constexpr color white{ max, max, max };
constexpr color red{ max, min, min };
constexpr color green{ min, max, min };
constexpr color blue{ min, min, max };
} // namespace colors

#endif // COLOR_HPP
