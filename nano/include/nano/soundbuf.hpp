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

    unsigned int frequence{ 0 };
    format_t fmt{ format_t::unknown };
    channel_t channel{ channel_t::unknown };
    std::uint8_t silence{ 0 };
};

class soundbuf
{
public:
    soundbuf() = default;
    ~soundbuf() = default;

    using bufdata_t = std::uint8_t;
    using buf_t = std::shared_ptr<bufdata_t>;

    soundbuf(buf_t, int, const audio_spec& = {});

    soundbuf(const soundbuf&);
    soundbuf& operator=(soundbuf);

    buf_t data();
    const buf_t data() const;

    int size() const;
    void size(int s);

    audio_spec& spec();
    const audio_spec& spec() const;

private:
    audio_spec m_spec;
    buf_t buf;
    int m_size{ 0 };
};

} // namespace nano

#endif // SOUNDBUF_HPP
