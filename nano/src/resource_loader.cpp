#include "SDL_pixels.h"
#include <nano/resource_loader.hpp>

#include <nano/error.hpp>
#include <nano/soundbuf.hpp>
#include <nano/utils.hpp>

#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_image.h>
#include <SDL3/SDL_rwops.h>

#include <cstdlib>
#include <iostream>
#include <istream>
#include <memory>

namespace nano::ppm
{

std::string
error2str(int e)
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
        return err_t::bad_stream;
    }
    if (fmt != "P6" and fmt != "P3")
    {
        return err_t::incorrect_format;
    }
    // clang-format off
    if (width > limits::MAX_IMAGE_SIZE or
        width < limits::MIN_IMAGE_SIZE or
        height > limits::MAX_IMAGE_SIZE or
        height < limits::MIN_IMAGE_SIZE or
        max_color_val != limits::MAX_COLOR_VALUE)
    {
        return err_t::violation_limits;
    }
    // clang-format on
    if (not std::iswspace(sep))
    {
        return err_t::incorrect_header;
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

    // img.transpose();

    if (src.bad())
    {
        return err_t::bad_stream;
    }
    return err_t::no_error;
}

int
dump(std::ostream& dst, const canvas& img, fmt format)
{
    if (not dst.good())
    {
        return err_t::bad_stream;
    }

    const std::size_t max_color_value =
        (1 << (8 * sizeof(color::channel_t))) - 1;

    dst << (format == fmt::P3 ? "P3" : "P6") << '\n'
        << img.width() << " " << img.height() << '\n'
        << max_color_value << '\n';

    if (format == fmt::P3)
    {
        for (std::size_t i{ 0 }; i < img.height(); ++i)
            for (std::size_t j{ 0 }; j < img.width(); ++j)
            {
                dst << static_cast<std::size_t>(img(i, j).r) << ' '
                    << static_cast<std::size_t>(img(i, j).g) << ' '
                    << static_cast<std::size_t>(img(i, j).b) << '\n';
            }
    }
    else // fmt == fmt::P6
    {
        dst.write(reinterpret_cast<const char*>(img.data()),
                  img.width() * img.height() * sizeof(color));
    }

    if (dst.bad())
    {
        return err_t::bad_stream;
    }
    return err_t::no_error;
}

} // namespace nano::ppm

namespace nano::image
{

std::string
error2str(int err)
{
    switch (err)
    {
    case err_t::no_error:
        return "INFO: no errors were occured";

    case err_t::internal_load:
        return "ERROR: Internal loading image error";

    case err_t::convert_pixelformat:
        return "ERROR: Convertiong pixels format error";

    default:
        return {};
    }
    return {};
}

int
load(const std::filesystem::path& fn, canvas& buf)
{
    SDL_Surface* img = IMG_Load(fn.string().c_str());
    ASSERT_IMG_ERROR(nullptr != img, err_t::internal_load);

    SDL_Surface* new_img{ nullptr };
    if (SDL_PIXELFORMAT_RGBA8888 != img->format->format)
    {
        LOG_DEBUG("Unsupported pixel format in image loaded from: %s",
                  path2str(fn).c_str());
        LOG_DEBUG("Trying to convert...");
        new_img = SDL_ConvertSurfaceFormat(img, SDL_PIXELFORMAT_RGB24);
        ASSERT_SDL_ERROR(nullptr != new_img, err_t::convert_pixelformat);
        LOG_DEBUG("Successful converting pixel format");
        SDL_DestroySurface(img);
        img = new_img;
    }

    buf.resize(img->w, img->h);
    std::memcpy(
        buf.data(), img->pixels, buf.height() * buf.width() * sizeof(color));
    SDL_DestroySurface(img);

    return err_t::no_error;
}

} // namespace nano::image

namespace nano::wav
{

std::string
error2str(int e)
{
    switch (e)
    {
    case err_t::no_error:
        return "INFO: no errors were occured";

    case err_t::bad_stream:
        return "Bad stream was provided in arguments, or fail when "
               "reading or writing";

    case err_t::calc_length:
        return "Failed while calculating length of stream buffer";

    case err_t::internal_read:
        return std::string("Failed while reading raw file data to "
                           "internal buffer:\n") +
               SDL_GetError();

    case err_t::incorrect_file_structure:
        return std::string("Failed while reading file wav structure:\n") +
               SDL_GetError();

    case err_t::unsupported:
        return "Unsupported specification data (format or channels)";

    default:
        return "Unknown error type was provided";
    }
}

int
load(const std::filesystem::path& fn, soundbuf& buf)
{
    SDL_AudioSpec spec;
    uint8_t* soundbuf_data{ nullptr };
    std::uint32_t soundbuf_len{ 0 };
    SDL_AudioSpec* res =
        SDL_LoadWAV(fn.string().c_str(), &spec, &soundbuf_data, &soundbuf_len);
    ASSERT_SDL_ERROR(nullptr != res, err_t::incorrect_file_structure);

    audio_spec soundbuf_spec;
    if (static_cast<int>(spec.channels) > 2)
    {
        return err_t::unsupported;
    }
    soundbuf_spec.channel = static_cast<audio_spec::channel_t>(spec.channels);
    // clang-format off
    if (spec.format != static_cast<SDL_AudioFormat>(audio_spec::format_t::f32) and
        spec.format != static_cast<SDL_AudioFormat>(audio_spec::format_t::s16))
    {
        return err_t::unsupported;
    }
    // clang-format on
    soundbuf_spec.fmt = static_cast<audio_spec::format_t>(spec.format);
    soundbuf_spec.frequence = spec.freq;
    soundbuf_spec.silence = spec.silence;

    // SDL use malloc for allocating memory,
    // that's why if I will use delete[] from shared_ptr<std::uint8_t[]>,
    // this will cause alloc-dealloc-mismatch
    buf = soundbuf(soundbuf::buf_t(soundbuf_data,
                                   [](auto&& p)
                                   {
                                       if (p)
                                           SDL_free(p);
                                   }),
                   soundbuf_len,
                   soundbuf_spec);

    return err_t::no_error;
}

} // namespace nano::wav
