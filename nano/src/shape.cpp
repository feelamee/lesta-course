#include <nano/shape.hpp>

#include <nano/engine.hpp>
#include <nano/error.hpp>
#include <nano/transform.hpp>

#include <glad/glad.h>

#include <cmath>

namespace nano
{

shape::shape(const vertbuf& p_vertbuf, const texture2D& p_texture)
    : vertices(p_vertbuf)
    , m_texture(p_texture)
{
}

const vertex*
shape::data() const
{
    return vertices.data();
}

vertex*
shape::data()
{
    return vertices.data();
}

void
shape::move(const vec2f& offset)
{
    position += offset;
    transform_need_update = true;
}

void
shape::scale(const vec2f& scale)
{
    factor = { factor.x * scale.x, factor.y * scale.y };
    transform_need_update = true;
}

void
shape::rotate(radian angle)
{
    rotation += angle;
    transform_need_update = true;
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

const texture2D&
shape::get_texture() const
{
    return m_texture;
}

const transform2D&
shape::get_transform() const
{
    if (transform_need_update)
    {
        const float sin = std::sin(rotation);
        const float cos = std::cos(rotation);

        const float _00 = factor.x * cos;
        const float _11 = factor.y * cos;
        const float _01 = factor.y * sin;
        const float _10 = -factor.x * sin;
        const float _02 = -origin.x * _00 - origin.y * _01 + position.x;
        const float _12 = -origin.x * _10 - origin.y * _11 + position.y;

        // clang-format off
        m_transform = transform2D(_00, _01, _02,
                                _10, _11, _12,
                                  0,   0,   1);
        // clang-format on
        transform_need_update = false;
    }
    return m_transform;
}

vec2f
shape::get_origin() const
{
    return origin;
}

vec2f
shape::get_position() const
{
    return position;
}

shape::radian
shape::get_rotation() const
{
    return rotation;
}

vec2f
shape::get_factor() const
{
    return factor;
}

void
render(const shape& p_shape)
{
    GL_CHECK(glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), &p_shape.data()->pos));
    GL_CHECK(glEnableVertexAttribArray(0));

    GL_CHECK(glVertexAttribPointer(1,
                                   4,
                                   GL_UNSIGNED_BYTE,
                                   GL_FALSE,
                                   sizeof(vertex),
                                   &p_shape.data()->rgb));
    GL_CHECK(glEnableVertexAttribArray(1));

    GL_CHECK(glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), &p_shape.data()->tpos));
    GL_CHECK(glEnableVertexAttribArray(2));

    GL_CHECK(glDrawArrays(static_cast<GLenum>(p_shape.primitive_type()),
                          0,
                          p_shape.points_count()));
}

} // namespace nano
