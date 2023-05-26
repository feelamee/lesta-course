#include <shape.hpp>

#include <glad/glad.h>

#include <cmath>

#include <glcheck.hpp>
#include <iengine.hpp>
#include <transform.hpp>

namespace nano
{

shape::shape(const vertbuf& p_vertbuf, texture* p_texture)
    : vertices(p_vertbuf)
    , m_texture(p_texture)
{
}

void
shape::set_transform(const transform2D& p_transform)
{
    m_transform = p_transform;
}

void
shape::set_texture(texture* p_texture)
{
    m_texture = p_texture;
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

texture*
shape::get_texture()
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

const texture*
shape::get_texture() const
{
    return m_texture;
}

void
render(const shape& p_shape)
{
    auto tex = p_shape.get_texture();
    GLboolean is_exist{ false };
    GL_CHECK(is_exist = glIsTexture(tex->get_handle()));
    if (GL_FALSE == is_exist)
    {
        err << tex->get_handle() << " isn't texture" << std::endl;
        return;
    }

    engine_instance().set_uniform("u_texture", tex);
    engine_instance().set_uniform("u_matrix", &p_shape.get_transform());
    GL_CHECK(glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), &p_shape.data()->pos));
    GL_CHECK(glEnableVertexAttribArray(0));

    GL_CHECK(glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), &p_shape.data()->color));
    GL_CHECK(glEnableVertexAttribArray(1));

    GL_CHECK(glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), &p_shape.data()->tpos));
    GL_CHECK(glEnableVertexAttribArray(2));

    GL_CHECK(glDrawArrays(static_cast<GLenum>(p_shape.primitive_type()),
                          0,
                          p_shape.points_count()));
}

} // namespace nano
