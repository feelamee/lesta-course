#ifndef IMAGE_LOADER_HPP
#define IMAGE_LOADER_HPP

#include <filesystem>
#include <nano/canvas.hpp>
#include <nano/soundbuf.hpp>

#include <cstdint>
#include <iosfwd>
#include <string>

namespace nano::ppm
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

enum err_t
{
    no_error = 0,
    bad_stream = -4,
    incorrect_format,
    violation_limits,
    incorrect_header,
};

std::string error2str(int e);

int load(std::istream& src, canvas& img);

int dump(std::ostream& dst, const canvas& img, fmt format);

} // namespace nano::ppm

namespace nano::image
{
enum err_t
{
    no_error = 0,
    internal_load = -2,
    convert_pixelformat,
};

std::string error2str(int);

int load(const std::filesystem::path& fn, canvas& buf);

} // namespace nano::image

namespace nano::wav
{

enum err_t
{
    no_error = 0,
    bad_stream = -5,
    calc_length,
    internal_read,
    incorrect_file_structure,
    unsupported,
};

std::string error2str(int);

int load(std::istream& src, soundbuf& buf);

} // namespace nano::wav

#endif // IMAGE_LOADER_HPP
