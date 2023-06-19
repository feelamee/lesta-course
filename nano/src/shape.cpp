#include "nano/texture2D.hpp"
#include <cstdlib>
#include <nano/shape.hpp>

#include <nano/engine.hpp>
#include <nano/error.hpp>
#include <nano/transform2D.hpp>

#include <glad/glad.h>

#include <cmath>

namespace nano
{

shape::shape(const vertbuf& p_vertbuf, const texture2D& p_texture)
    : vertices(p_vertbuf)
    , m_texture(p_texture)
{
    auto& win_size = engine::instance()->window.size;
    scale(1, win_size.x / win_size.y);
    origin({ 0, texture().size().normalized().y });
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
    position(m_position + offset);
}

void
shape::scale(const vec2f& scale)
{
    factor({ m_factor.x * scale.x, m_factor.y * scale.y });
}

void
shape::rotate(radian angle)
{
    rotation(m_rotation + angle);
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
shape::texture() const
{
    return m_texture;
}

const transform2D&
shape::transform() const
{
    if (transform_need_update)
    {
        transform2D new_tr;
        new_tr.rotate(rotation(), origin()).move(position()).scale(factor());
        m_transform = new_tr;

        transform_need_update = false;
    }
    return m_transform;
}

vec2f
shape::origin() const
{
    return m_origin;
}

vec2f
shape::position() const
{
    return m_position;
}

shape::radian
shape::rotation() const
{
    return m_rotation;
}

vec2f
shape::factor() const
{
    return m_factor;
}

void
shape::origin(const vec2f& p_origin)
{
    m_origin = p_origin;
    transform_need_update = true;
}

void
shape::position(const vec2f& p_posiiton)
{
    m_position = p_posiiton;
    transform_need_update = true;
}

void
shape::rotation(radian p_rotation)
{
    m_rotation = p_rotation;
    transform_need_update = true;
}

void
shape::factor(const vec2f& p_factor)
{
    m_factor = p_factor;
    transform_need_update = true;
}

void
shape::move(vec2f::type offset_x, vec2f::type offset_y)
{
    move({ offset_x, offset_y });
}

void
shape::scale(vec2f::type scale_x, vec2f::type scale_y)
{
    scale({ scale_x, scale_y });
}

void
shape::draw(const state& s) const
{
    transform2D tr = s.transform;
    tr.combine(transform());

    if (s.program)
    {
        s.program->uniform("u_texture", texture());
        s.program->uniform("u_matrix", tr);
        shader::use(*s.program);
    }
    else
    {
        LOG_DEBUG("Shader need for draw sprite");
        return;
    };
    vertices.bind_vbo();
}

} // namespace nano
