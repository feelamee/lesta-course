#include <glad/glad.h>
#include <vertbuf.hpp>

namespace nano
{

vertbuf::vertbuf(primitive_t p_type, const std::vector<vertex>& p_vertices)
    : type(p_type)
    , vertices(p_vertices)
{
}

std::size_t
vertbuf::size() const
{
    return vertices.size();
}

primitive_t
vertbuf::primitive_type() const
{
    return type;
}

vertex*
vertbuf::data()
{
    return vertices.data();
}

const vertex*
vertbuf::data() const
{
    return vertices.data();
}

void
render(const vertbuf& vertices)
{
    glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE, sizeof(nano::vertex), vertices.data());
    glEnableVertexAttribArray(0);

    glDrawArrays(
        static_cast<GLenum>(vertices.primitive_type()), 0, vertices.size());
}

} // namespace nano
