#ifndef LOAD_HPP
#define LOAD_HPP

#include "canvas.hpp"
#include "color.hpp"

#include <boost/numeric/ublas/storage.hpp>
#include <cstdint>
#include <ranges>
#include <string>

namespace ppm
{

namespace vs = std::views;

enum class fmt
{
    P3,
    P6,
};

enum limits
{
    MAX_IMAGE_SIZE = 1U << 12,
    MAX_COLOR_VALUE = (1U << 16) - 1,
};

int
load(std::istream& src, canvas& img)
{
    if (!src.good())
        return EXIT_FAILURE;

    std::string fmt;
    size_t width = 0, height = 0;
    size_t max_color_val = 0;
    char sep = '\0';

    src >> fmt >> width >> height >> max_color_val;
    sep = src.get();
    if ((fmt != "P6" and fmt != "P3") or width > limits::MAX_IMAGE_SIZE or
        height > limits::MAX_IMAGE_SIZE or
        max_color_val > limits::MAX_COLOR_VALUE or
        ((1U << 8 * sizeof(color_channel_t<>)) < max_color_val + 1) or
        !iswspace(sep))
        return EXIT_FAILURE;

    img.resize(width, height, false);

    if (fmt == "P6")
    {
        src.read(reinterpret_cast<char*>(img.data().data().begin()),
                 img.width() * img.height() * sizeof(color));
    }
    else
    {
        size_t r, g, b;
        for (auto i : vs::iota(0UL, img.width()))
            for (auto j : vs::iota(0UL, img.height()))
            {
                src >> r >> g >> b;
                img(i, j) = { static_cast<color_channel_t<>>(r),
                              static_cast<color_channel_t<>>(g),
                              static_cast<color_channel_t<>>(b) };
            }
    }

    return src ? EXIT_SUCCESS : EXIT_FAILURE;
}

template <fmt fmt>
int
dump(std::ostream& dst, const canvas& img)
{
    if (!dst.good())
        return EXIT_FAILURE;

    const size_t max_color_value = (1 << 8 * sizeof(color_channel_t<>)) - 1;
    dst << (fmt == fmt::P3 ? "P3" : "P6") << std::endl
        << img.width() << " " << img.height() << std::endl
        << max_color_value << std::endl;

    if constexpr (fmt == fmt::P3)
    {
        for (auto i : vs::iota(0UL, img.height()))
            for (auto j : vs::iota(0UL, img.width()))
            {
                dst << static_cast<size_t>(img(j, i).r) << " "
                    << static_cast<size_t>(img(j, i).g) << " "
                    << static_cast<size_t>(img(j, i).b) << std::endl;
            }
    }
    else // fmt == fmt::P6
    {
        dst.write(reinterpret_cast<const char*>(img.data().data().begin()),
                  img.width() * img.height() * sizeof(color));
    }

    return dst ? EXIT_SUCCESS : EXIT_FAILURE;
}

} // namespace ppm

#endif // LOAD_HPP
