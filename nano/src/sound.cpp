#include <nano/sound.hpp>

#include <nano/error.hpp>
#include <nano/resource_loader.hpp>

#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_log.h>

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <limits>

namespace nano
{

static bool
operator==(const SDL_AudioSpec& lhs, const SDL_AudioSpec& rhs)
{
    // clang-format off
    return lhs.format == rhs.format &&
           lhs.channels == rhs.channels &&
           lhs.freq == rhs.freq &&
           lhs.silence == rhs.silence &&
           lhs.callback == rhs.callback;
    // clang-format on
}

int
sound::load(const std::filesystem::path& fn)
{
    std::ifstream file(fn, std::ios::binary);
    if (not file)
    {
        LOG_DEBUG("ERROR: failed while opening file: %s", fn.string().c_str());
        return EXIT_FAILURE;
    }

    soundbuf buf;
    int err_code = nano::wav::load(file, buf);
    if (EXIT_SUCCESS != err_code)
    {
        LOG_DEBUG("ERROR: failed while loading file %s\n%s",
                  fn.string().c_str(),
                  nano::wav::error2str(err_code).c_str());
        return EXIT_FAILURE;
    }

    err_code = load(buf);
    if (EXIT_SUCCESS != err_code)
    {
        LOG_DEBUG("ERROR: failed while loading sound from soundbuf");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int
sound::load(const soundbuf& p_buf, std::size_t p_position)
{
    buf = p_buf;

    if (audio_deviceID > 0)
    {
        return EXIT_SUCCESS;
    }

    const nano::audio_spec& _spec = spec();
    SDL_AudioSpec obtained;
    SDL_AudioSpec spec{ .freq = static_cast<int>(_spec.frequence),
                        .format = static_cast<SDL_AudioFormat>(_spec.fmt),
                        .channels = static_cast<Uint8>(_spec.channel),
                        .silence = _spec.silence };
    spec.callback = audio_callback;
    spec.userdata = this;
    audio_deviceID = SDL_OpenAudioDevice(nullptr, 0, &spec, &obtained, 0);
    if (0 == audio_deviceID)
    {
        LOG_DEBUG("Failed to open audio device: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    if (obtained != spec)
    {
        LOG_DEBUG("Obtained spec is not equal to desired\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void
sound::position(std::size_t p_position)
{
    m_position = p_position;
}

std::size_t
sound::position() const
{
    return m_position;
}

soundbuf::buf_t
sound::data()
{
    return buf.data();
}
const soundbuf::buf_t
sound::data() const
{
    return buf.data();
}

std::size_t
sound::size() const
{
    return std::clamp(buf.size(), 0, std::numeric_limits<int>::max());
}

// TODO: implement
sound::msduration
sound::advance(msduration dur)
{
    return {};
}

void
sound::play()
{
    TEST_SDL_ERROR(EXIT_SUCCESS == SDL_PlayAudioDevice(audio_deviceID));
}

void
sound::pause()
{
    TEST_SDL_ERROR(EXIT_SUCCESS == SDL_PauseAudioDevice(audio_deviceID));
}

void
sound::stop()
{
    TEST_SDL_ERROR(EXIT_SUCCESS == SDL_PauseAudioDevice(audio_deviceID));
    position(0);
}

void
sound::toggle()
{
    SDL_AudioStatus status = SDL_GetAudioDeviceStatus(audio_deviceID);
    if (SDL_AUDIO_STOPPED == status or SDL_AUDIO_PAUSED == status)
    {
        play();
    }
    else
    {
        pause();
    }
}

sound::status_t
sound::status() const
{
    return static_cast<status_t>(SDL_GetAudioDeviceStatus(audio_deviceID));
}

const audio_spec&
sound::spec() const
{
    return buf.spec();
}

void
sound::volume(int val)
{
    m_volume = std::clamp(val, 0, SDL_MIX_MAXVOLUME);
}

int
sound::volume() const
{
    return m_volume;
}

void
sound::audio_callback(void* userdata, std::uint8_t* stream, int len)
{
    auto buf = static_cast<sound*>(userdata);
    std::fill_n(stream, len, buf->spec().silence);
    int pos = buf->position();
    int left = buf->size() - pos;

    if (left > len)
    {
        SDL_MixAudioFormat(stream,
                           buf->data().get() + pos,
                           static_cast<SDL_AudioFormat>(buf->spec().fmt),
                           len,
                           buf->volume());
        buf->position(pos + len);
    }
    else
    {
        SDL_MixAudioFormat(stream,
                           buf->data().get() + pos,
                           static_cast<SDL_AudioFormat>(buf->spec().fmt),
                           left,
                           buf->volume());
        if (buf->loop)
        {
            buf->position(0);
        }
    }
}

} // namespace nano
