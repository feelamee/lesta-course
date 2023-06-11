#include <resource_loader.hpp>

#include <SDL3/SDL_audio.h>

#include <cstdlib>
#include <iostream>

namespace ppm
{

static err_t err;

err_t
error()
{
    err_t ret = err;
    err = err_t::no_error;
    return ret;
}

std::string
error2str(err_t e)
{
    switch (e)
    {
    case err_t::no_error:
        return "INFO: no errors were occured";

    case err_t::bad_stream:
        return "ERROR: bad stream was provided in arguments, or fail when "
               "reading or writing";

    case err_t::incorrect_format:
        return "ERROR: incorrect format, it should be P3 or P6";

    case err_t::violation_limits:
        return "ERROR: image width, height or max color value are out of "
               "bounds";

    case err_t::incorrect_header:
        return "ERROR: header formatting is incorrect";

    default:
        return "unknown error type was provided";
    }
}

int
load(std::istream& src, canvas& img)
{
    std::string fmt;
    std::size_t width{ 0 }, height{ 0 };
    std::size_t max_color_val{ 0 };
    char sep{ '\0' };

    src >> fmt;
    src >> width;
    src >> height;
    src >> max_color_val;
    sep = src.get();

    if (not src.good())
    {
        err = err_t::bad_stream;
        return EXIT_FAILURE;
    }
    if (fmt != "P6" and fmt != "P3")
    {
        err = err_t::incorrect_format;
        return EXIT_FAILURE;
    }
    // clang-format off
    if (width > limits::MAX_IMAGE_SIZE or
        width < limits::MIN_IMAGE_SIZE or
        height > limits::MAX_IMAGE_SIZE or
        height < limits::MIN_IMAGE_SIZE or
        max_color_val != limits::MAX_COLOR_VALUE)
    {
        err = err_t::violation_limits;
        return EXIT_FAILURE;
    }
    // clang-format on
    if (not std::iswspace(sep))
    {
        err = err_t::incorrect_header;
        return EXIT_FAILURE;
    }

    img.resize(static_cast<std::size_t>(width),
               static_cast<std::size_t>(height));

    if (fmt == "P6")
    {
        src.read(reinterpret_cast<char*>(img.data()),
                 img.width() * img.height() * sizeof(color));
    }
    else
    {
        std::size_t r, g, b;
        for (std::size_t i{ 0 }; i < img.height(); ++i)
            for (std::size_t j{ 0 }; j < img.width(); ++j)
            {
                src >> r >> g >> b;
                img(i, j) = { static_cast<color::channel_t>(r),
                              static_cast<color::channel_t>(g),
                              static_cast<color::channel_t>(b) };
            }
    }

    if (src.bad())
    {
        err = err_t::bad_stream;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int
dump(std::ostream& dst, const canvas& img, fmt format)
{
    if (not dst.good())
    {
        err = err_t::bad_stream;
        return EXIT_FAILURE;
    }

    const std::size_t max_color_value =
        (1 << (8 * sizeof(color::channel_t))) - 1;

    dst << (format == fmt::P3 ? "P3 " : "P6 ");
    dst << img.width();
    dst << ' ';
    dst << img.height();
    dst << ' ';
    dst << max_color_value;
    dst << ' ';

    if (format == fmt::P3)
    {
        for (std::size_t i{ 0 }; i < img.height(); ++i)
            for (std::size_t j{ 0 }; j < img.width(); ++j)
            {
                dst << static_cast<std::size_t>(img(i, j).r) << ' '
                    << static_cast<std::size_t>(img(i, j).g) << ' '
                    << static_cast<std::size_t>(img(i, j).b) << ' ';
            }
    }
    else if (format == fmt::P6)
    {
        dst.write(reinterpret_cast<const char*>(img.data()),
                  img.width() * img.height() * sizeof(color));
    }

    if (not dst)
    {
        err = err_t::bad_stream;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

} // namespace ppm
