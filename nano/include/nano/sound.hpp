#ifndef SOUND_HPP
#define SOUND_HPP

#include <nano/soundbuf.hpp>

#include <filesystem>

namespace nano
{

class sound
{
public:
    enum class status_t
    {
        stopped = 0,
        paused,
        playing
    };

    sound() = default;

    int load(const std::filesystem::path& fn);
    int load(const soundbuf& p_buf, std::size_t p_position = 0);

    void position(std::size_t p_position);
    std::size_t position() const;
    int advance(std::size_t distance);

    status_t status() const;
    void play() const;
    void pause() const;
    void toggle() const;

    void volume(int val);
    int volume() const;

    const audio_spec& spec() const;

    std::size_t size() const;

    soundbuf::buf_t data();
    const soundbuf::buf_t data() const;

    static void audio_callback(void* userdata, std::uint8_t* stream, int len);

private:
    std::uint32_t audio_deviceID;
    soundbuf buf;
    std::size_t m_position{ 0 };
    std::uint8_t m_volume{ 0 };
};

} // namespace nano

#endif // SOUND_HPP
