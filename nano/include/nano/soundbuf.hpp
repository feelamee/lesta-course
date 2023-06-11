#ifndef SOUNDBUF_HPP
#define SOUNDBUF_HPP

#include <cstdint>
#include <memory>

namespace nano
{

struct audio_spec
{
    enum class channel_t
    {
        unknown = 0,
        mono = 1,
        stereo = 2,
    };

    enum class format_t
    {
        unknown = 0,
        s16 = 0x8010,
        f32 = 0x8120,
    };

    unsigned int frequence;
    format_t fmt;
    channel_t channel;
    std::uint8_t silence;
};

class soundbuf
{
public:
    soundbuf();
    soundbuf(std::shared_ptr<uint8_t[]>, int, const audio_spec& = {});

    soundbuf(const soundbuf&);
    soundbuf(soundbuf&&);
    soundbuf& operator=(soundbuf);

    ~soundbuf();
    std::uint8_t* data();
    const std::uint8_t* const data() const;

    int size() const;
    void size(int s);

    audio_spec& specification();
    const audio_spec& specification() const;

private:
    audio_spec spec;
    std::shared_ptr<std::uint8_t[]> buf;
    int m_size{ 0 };
};

} // namespace nano

#endif // SOUNDBUF_HPP
