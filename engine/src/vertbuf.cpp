#include <vertbuf.hpp>

namespace nano
{

vec2i
vertbuf::size()
{
    return m_size;
}

primitive_t
vertbuf::primitive_type()
{
    return type;
}

vertex*
vertbuf::data()
{
    return vertices.data();
}

} // namespace nano
