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

soundbuf::~soundbuf()
{
    if (nullptr != buf)
        delete[] buf;
}

soundbuf::soundbuf(soundbuf&& other)
{
    if (nullptr != buf)
        delete[] buf;

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

std::uint8_t*&
soundbuf::data()
{
    return buf;
}

const std::uint8_t* const
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
