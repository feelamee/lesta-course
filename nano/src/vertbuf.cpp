#include <nano/vertbuf.hpp>

#include <glad/glad.h>
#include <nano/error.hpp>

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
    GL_CHECK(glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), &vertices.data()->pos));
    GL_CHECK(glEnableVertexAttribArray(0));

    GL_CHECK(glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), &vertices.data()->color));
    GL_CHECK(glEnableVertexAttribArray(1));

    GL_CHECK(glDrawArrays(
        static_cast<GLenum>(vertices.primitive_type()), 0, vertices.size()));
}

} // namespace nano
