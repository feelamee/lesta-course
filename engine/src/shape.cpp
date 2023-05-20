#include <shape.hpp>

#include <cmath>
#include <errors.hpp>
#include <glad/glad.h>
#include <iengine.hpp>
#include <transform.hpp>

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

const auto
shape::data() const
{
    return vertices.data();
}

auto
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
    factor = { factor.fst * scale.fst, factor.snd * scale.snd };
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
    return &m_texture;
}

const transform&
shape::get_transform() const
{
    if (transform_need_update)
    {
        const float sin = std::sin(rotation);
        const float cos = std::cos(rotation);

        const float _00 = factor.fst * cos;
        const float _11 = factor.snd * cos;
        const float _01 = factor.snd * sin;
        const float _10 = -factor.fst * sin;
        const float _02 = -origin.fst * _00 - origin.snd * _01 + position.fst;
        const float _12 = -origin.fst * _10 - origin.snd * _11 + position.snd;

        // clang-format off
        m_transform = transform(_00, _01, _02,
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
    return &m_texture;
}

void
render(const shape& p_shape)
{
    auto tex = p_shape.get_texture();
    if (GL_FALSE == glIsTexture(tex->get_handle()))
    {
        std::cerr << tex->get_handle() << " isn't texture" << std::endl;
        return;
    }
    engine_instance().set_uniform("u_texture", tex);
    engine_instance().set_uniform("u_matrix", &p_shape.get_transform());
    glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), &p_shape.data()->pos);
    OM_GL_CHECK();
    glEnableVertexAttribArray(0);
    OM_GL_CHECK();

    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), &p_shape.data()->color);
    OM_GL_CHECK();
    glEnableVertexAttribArray(1);
    OM_GL_CHECK();

    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), &p_shape.data()->tpos);
    OM_GL_CHECK();
    glEnableVertexAttribArray(2);
    OM_GL_CHECK();

    glDrawArrays(static_cast<GLenum>(p_shape.primitive_type()),
                 0,
                 p_shape.points_count());
    OM_GL_CHECK();
}

} // namespace nano
