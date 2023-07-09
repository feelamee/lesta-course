#include <nano/vertbuf.hpp>

#include <nano/error.hpp>

#include <glad/glad.h>

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

void
vertbuf::bind_vbo() const
{
    GL_CHECK(glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), &data()->pos));
    GL_CHECK(glEnableVertexAttribArray(0));

    GL_CHECK(glVertexAttribPointer(
        1, 3, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(vertex), &data()->rgb));
    GL_CHECK(glEnableVertexAttribArray(1));

    GL_CHECK(glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), &data()->tpos));
    GL_CHECK(glEnableVertexAttribArray(2));
}

void
vertbuf::draw() const
{
    bind_vbo();
    GL_CHECK(glDrawArrays(static_cast<GLenum>(primitive_type()), 0, size()));
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

    GL_CHECK(glVertexAttribPointer(1,
                                   4,
                                   GL_UNSIGNED_BYTE,
                                   GL_FALSE,
                                   sizeof(vertex),
                                   &vertices.data()->rgb));
    GL_CHECK(glEnableVertexAttribArray(1));

    GL_CHECK(glDrawArrays(
        static_cast<GLenum>(vertices.primitive_type()), 0, vertices.size()));
}

} // namespace nano
