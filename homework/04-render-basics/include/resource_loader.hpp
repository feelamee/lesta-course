#ifndef IMAGE_LOADER_HPP
#define IMAGE_LOADER_HPP

#include <canvas.hpp>

#include <cstdint>
#include <iosfwd>
#include <string>

namespace ppm
{

enum class fmt
{
    P3,
    P6,
};

enum limits
{
    MAX_IMAGE_SIZE = 1U << 12,
    MIN_IMAGE_SIZE = 0,
    MAX_COLOR_VALUE = (1 << 8 * sizeof(color::channel_t)) - 1,
    MIN_COLOR_VALUE = 0,
};

enum class err_t
{
    no_error,
    bad_stream,
    incorrect_format,
    violation_limits,
    incorrect_header,
};

err_t error();

std::string error2str(err_t e);

int load(std::istream& src, canvas& img);

int dump(std::ostream& dst, const canvas& img, fmt format);

} // namespace ppm

#endif // IMAGE_LOADER_HPP
