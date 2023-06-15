#include <cstdint>
#include <limits>
#include <nano/soundbuf.hpp>

#include <algorithm>
#include <climits>

namespace nano
{

soundbuf::soundbuf(buf_t p_buf, int p_size, const audio_spec& p_spec)
{
    buf = p_buf;
    size(p_size);
    m_spec = p_spec;
}

soundbuf::soundbuf(const soundbuf& other)
{
    buf = other.buf;
    size(other.size());
    m_spec = other.m_spec;
}

soundbuf&
soundbuf::operator=(soundbuf other)
{
    std::swap(this->buf, other.buf);
    std::swap(this->m_size, other.m_size);
    std::swap(this->m_spec, other.m_spec);
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
    m_size = std::clamp(s, 0, std::numeric_limits<int>::max());
}

soundbuf::buf_t
soundbuf::data()
{
    return buf;
}

const soundbuf::buf_t
soundbuf::data() const
{
    return buf;
}

audio_spec&
soundbuf::spec()
{
    return m_spec;
}

const audio_spec&
soundbuf::spec() const
{
    return m_spec;
}

} // namespace nano
