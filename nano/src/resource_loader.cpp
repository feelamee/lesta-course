#include <memory>
#include <nano/resource_loader.hpp>

#include <nano/soundbuf.hpp>

#include <SDL3/SDL_audio.h>

#include <cstdlib>
#include <iostream>
#include <istream>

namespace nano::ppm
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

    src >> fmt >> width >> height >> max_color_val;
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

    img.transpose();

    if (not src)
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

    dst << (format == fmt::P3 ? "P3" : "P6") << std::endl
        << img.width() << " " << img.height() << std::endl
        << max_color_value << std::endl;

    if (format == fmt::P3)
    {
        for (std::size_t i{ 0 }; i < img.height(); ++i)
            for (std::size_t j{ 0 }; j < img.width(); ++j)
            {
                dst << static_cast<std::size_t>(img(i, j).r) << " "
                    << static_cast<std::size_t>(img(i, j).g) << " "
                    << static_cast<std::size_t>(img(i, j).b) << std::endl;
            }
    }
    else // fmt == fmt::P6
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

} // namespace nano::ppm

namespace nano::wav
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

    case err_t::calc_length:
        return "ERROR: failed while calculating length of stream buffer";

    case err_t::internal_read:
        return std::string("ERROR: failed while reading raw file data to "
                           "internal buffer:\n") +
               SDL_GetError();

    case err_t::incorrect_file_structure:
        return std::string(
                   "ERROR: failed while reading file wav structure:\n") +
               SDL_GetError();

    case err_t::unsupported:
        return "ERROR: unsupported specification data (format or channels)";

    default:
        return "unknown error type was provided";
    }
}

int
load(std::istream& src, soundbuf& buf)
{
    if (not src.good())
    {
        err = err_t::bad_stream;
        return EXIT_FAILURE;
    }
    src.seekg(0, std::ios::end);
    int size = src.tellg();
    if (-1 == size)
    {
        err = err_t::calc_length;
        return EXIT_FAILURE;
    }
    src.seekg(0, std::ios::beg);
    auto raw_data = new std::uint8_t[size];
    src.read(reinterpret_cast<char*>(raw_data), size);

    SDL_RWops* sdl_buf = SDL_RWFromMem(raw_data, size);
    if (nullptr == sdl_buf)
    {
        err = err_t::internal_read;
        delete[] raw_data;
        return EXIT_FAILURE;
    }

    SDL_AudioSpec spec;
    std::uint8_t* buf_data{ nullptr };
    std::uint32_t buf_len{ 0 };
    SDL_AudioSpec* res = SDL_LoadWAV_RW(sdl_buf, 1, &spec, &buf_data, &buf_len);
    buf.data() = buf_data;
    if (nullptr == res)
    {
        err = err_t::incorrect_file_structure;
        return EXIT_FAILURE;
    }
    buf.size(buf_len);

    audio_spec& buf_spec = buf.specification();
    if (static_cast<int>(spec.channels) > 2)
    {
        err = err_t::unsupported;
        return EXIT_FAILURE;
    }
    buf_spec.channel = static_cast<audio_spec::channel_t>(spec.channels);
    // clang-format off
    if (spec.format != static_cast<SDL_AudioFormat>(audio_spec::format_t::f32) and
        spec.format != static_cast<SDL_AudioFormat>(audio_spec::format_t::s16))
    // clang-format on
    {
        err = err_t::unsupported;
        return EXIT_FAILURE;
    }
    buf_spec.fmt = static_cast<audio_spec::format_t>(spec.format);
    buf_spec.frequence = spec.freq;
    buf_spec.silence = spec.silence;

    return EXIT_SUCCESS;
}

} // namespace nano::wav
