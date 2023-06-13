#include <nano/soundbuf.hpp>

namespace nano
{

soundbuf::soundbuf()
{
    spec.channel = audio_spec::channel_t::unknown;
    spec.fmt = audio_spec::format_t::unknown;
    spec.frequence = {};
    spec.silence = {};
}

soundbuf::soundbuf(std::shared_ptr<uint8_t[]> p_buf,
                   int p_size,
                   const audio_spec& p_spec)
{
    buf = p_buf;
    size(p_size);
    spec = p_spec;
}

soundbuf::~soundbuf()
{
}

soundbuf::soundbuf(const soundbuf& other)
{
    buf = other.buf;
    size(other.size());
    spec = other.spec;
}

soundbuf::soundbuf(soundbuf&& other)
{
    buf = other.buf;
    size(other.size());
    other.buf = nullptr;
    spec = other.spec;
}

soundbuf&
soundbuf::operator=(soundbuf other)
{
    std::swap(this->buf, other.buf);
    std::swap(this->m_size, other.m_size);
    std::swap(this->spec, other.spec);
    return *this;
}

int
soundbuf::size() const
{
    return m_size;
}

void
soundbuf::size(int s)
{
    if (s >= 0)
        m_size = s;
    else
        m_size = 0;
}

std::shared_ptr<std::uint8_t[]>
soundbuf::data()
{
    return buf;
}

const std::shared_ptr<std::uint8_t[]>
soundbuf::data() const
{
    return buf;
}

audio_spec&
soundbuf::specification()
{
    return spec;
}

const audio_spec&
soundbuf::specification() const
{
    return spec;
}

} // namespace nano
