#include "SDL_audio.h"
#include "nano/soundbuf.hpp"
#include <cstdlib>
#include <fstream>
#include <nano/resource_loader.hpp>

#include <SDL3/SDL.h>

#include <cstdint>
#include <experimental/scope>
#include <iostream>
#include <memory>

bool
operator==(const SDL_AudioSpec& lhs, const SDL_AudioSpec& rhs)
{
    // clang-format off
    return lhs.format == rhs.format &&
           lhs.channels == rhs.channels &&
           lhs.freq == rhs.freq &&
           lhs.silence == rhs.silence &&
           lhs.callback == rhs.callback;
           //lhs.samples == rhs.samples;
           //lhs.size == rhs.size;   // sizeof audio buffer size,
                                         // we dont know it when create desired spec
    // clang-format on
}

std::ostream&
operator<<(std::ostream& o, const SDL_AudioSpec& spec)
{
    std::string tab(4, ' ');
    o << tab << "freq: " << spec.freq << '\n'
      << tab << "format: " << std::hex << spec.format << '\n'
      << tab << "channels: " << std::dec << int(spec.channels) << '\n'
      << tab << "silence: " << int(spec.silence) << '\n'
      << tab << "samples: " << spec.samples << '\n'
      << tab << "size: " << spec.size << '\n'
      << tab << "callback: " << reinterpret_cast<const void*>(spec.callback)
      << '\n'
      << tab << "userdata: " << spec.userdata << std::endl;
    return o;
}

#pragma pack(push, 1)
struct sound
{
    nano::soundbuf buf;
    size_t position = 0;
};
#pragma pack(pop)
void
audio_callback(void* userdata, std::uint8_t* stream, int len)
{
    auto buf = (sound*)userdata;
    std::fill_n(stream, len, 0);
    int left = buf->buf.size() - buf->position;

    if (left > len)
    {
        SDL_MixAudioFormat(
            stream,
            buf->buf.data() + buf->position,
            static_cast<SDL_AudioFormat>(buf->buf.specification().fmt),
            len,
            SDL_MIX_MAXVOLUME);
        buf->position += len;
    }
    else
    {
        SDL_MixAudioFormat(
            stream,
            buf->buf.data() + buf->position,
            static_cast<SDL_AudioFormat>(buf->buf.specification().fmt),
            left,
            SDL_MIX_MAXVOLUME);
        buf->position = 0;
    }
}

int
main()
{
    int err = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO);
    if (EXIT_FAILURE == err)
    {
        SDL_Log("%s", SDL_GetError());
        return err;
    }
    std::experimental::scope_exit sdl_quit(SDL_Quit);

    // const char* fn = "../homework/06-and-god-create-sounds/road.wav";
    const char* fn = "../homework/06-and-god-create-sounds/highlands.wav";

    // Uint8* buf{ nullptr };
    // Uint32 buf_len{ 0 };
    // SDL_AudioSpec spec;
    // SDL_AudioSpec* spec_ptr = SDL_LoadWAV(fn, &spec, &buf, &buf_len);
    // std::unique_ptr<Uint8> buf_unique_ptr(buf);

    // if (nullptr == spec_ptr)
    // {
    //     SDL_LogError(SDL_LOG_CATEGORY_AUDIO,
    //                  "Can't parse and load audio samples from file %s\n",
    //                  fn);
    //     return EXIT_FAILURE;
    // }

    // spec.callback = audio_callback;
    // spec.userdata = &sbuf;

    sound sbuf;
    std::ifstream file(fn);
    if (not file)
    {
        std::cerr << "ERROR: failed while opening file:\n\t" << fn << std::endl;
        return EXIT_FAILURE;
    }
    int err_code = nano::wav::load(file, sbuf.buf);
    if (EXIT_FAILURE == err_code)
    {
        using namespace nano::wav;
        std::cerr << "ERROR: failed while loading file:\n\t" << fn << std::endl;
        std::cerr << error2str(error()) << std::endl;
        return EXIT_FAILURE;
    }

    const nano::audio_spec& _spec = sbuf.buf.specification();
    SDL_AudioSpec obtained;
    SDL_AudioSpec spec{ .freq = static_cast<int>(_spec.frequence),
                        .format = static_cast<SDL_AudioFormat>(_spec.fmt),
                        .channels = static_cast<Uint8>(_spec.channel),
                        .silence = _spec.silence };
    spec.callback = audio_callback;
    spec.userdata = &sbuf;
    SDL_AudioDeviceID audio_device =
        SDL_OpenAudioDevice(nullptr, 0, &spec, &obtained, 0);

    if (0 == audio_device)
    {
        SDL_LogError(SDL_LOG_CATEGORY_AUDIO,
                     "Failed to open audio device: %s",
                     SDL_GetError());
        return EXIT_FAILURE;
    }

    if (obtained != spec)
    {
        SDL_LogError(SDL_LOG_CATEGORY_AUDIO,
                     "Obtained spec is not equal to desired");
        std::cerr << "obtained: \n" << obtained << std::endl;
        std::cerr << "desired: \n" << spec << std::endl;
        return EXIT_FAILURE;
    }

    if (EXIT_SUCCESS != SDL_PlayAudioDevice(audio_device))
    {
        SDL_LogError(SDL_LOG_CATEGORY_AUDIO,
                     "Failed to play audio device %d.\n%s",
                     audio_device,
                     SDL_GetError());
        return EXIT_FAILURE;
    }

    while (true)
    {
        char c;
        std::cin >> c;
        if ('q' == c)
            return EXIT_SUCCESS;
    }

    return 0;
}
