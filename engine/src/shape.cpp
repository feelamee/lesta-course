#include "iengine.hpp"
#include "shader.hpp"
#include <glad/glad.h>
#include <shape.hpp>
#include <vertbuf.hpp>

namespace nano
{

shape::shape(const vertbuf& p_vertbuf, const texture& p_texture)
    : vertices(p_vertbuf)
    , m_texture(p_texture)
{
}

void
shape::set_transform(const transform& p_transform)
{
    m_transform = p_transform;
}

void
shape::set_texture(const texture& p_texture)
{
    m_texture = p_texture;
}

void
shape::set_viewport(const rect<int>& p_viewport)
{
    viewport = p_viewport;
}

inline const auto
shape::data() const
{
    return vertices.data();
}

inline auto
shape::data()
{
    return vertices.data();
}

inline void
shape::move(const vec2f& offset)
{
    m_transform.move(offset);
}

inline void
shape::scale(const vec2f& scale)
{
    m_transform.scale(scale);
}

inline void
shape::rotate(radian angle)
{
    m_transform.rotate(angle);
}

primitive_t
shape::primitive_type() const
{
    return vertices.primitive_type();
}

std::size_t
shape::points_count() const
{
    return vertices.size();
}

texture*
shape::get_texture()
{
    return &m_texture;
}

const texture*
shape::get_texture() const
{
    return &m_texture;
}

void
render(const shape& p_shape)
{
    auto tex = p_shape.get_texture();
    engine_instance().set_uniform("p_texture", tex);
    glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE, sizeof(nano::vertex), &p_shape.data()->pos);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, sizeof(nano::vertex), &p_shape.data()->color);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, sizeof(nano::vertex), &p_shape.data()->tpos);
    glEnableVertexAttribArray(2);

    glDrawArrays(static_cast<GLenum>(p_shape.primitive_type()),
                 0,
                 p_shape.points_count());
}

} // namespace nano
