#ifndef SOUND_HPP
#define SOUND_HPP

#include <chrono>
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

    using msduration = std::chrono::duration<std::int64_t, std::milli>;
    msduration advance(msduration dur);

    status_t status() const;
    void stop();
    void play();
    void pause();
    void toggle();

    void volume(int val);
    int volume() const;

    const audio_spec& spec() const;

    std::size_t size() const;

    soundbuf::buf_t data();
    const soundbuf::buf_t data() const;

    static void audio_callback(void* userdata, std::uint8_t* stream, int len);

private:
    std::uint32_t audio_deviceID{ 0 };
    soundbuf buf;
    std::size_t m_position{ 0 };
    std::uint8_t m_volume{ 0 };
};

} // namespace nano

#endif // SOUND_HPP
