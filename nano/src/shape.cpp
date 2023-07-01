#include <nano/shape.hpp>

#include <nano/engine.hpp>
#include <nano/error.hpp>
#include <nano/texture2D.hpp>
#include <nano/transform2D.hpp>
#include <nano/utils.hpp>

#include <glad/glad.h>

#include <cmath>
#include <cstdlib>
#include <memory>

namespace nano
{

shape::shape()
{
    auto& win = engine::instance()->window;
    scale(1, win.ratio);
}

shape::shape(const vertbuf& p_vertbuf, std::shared_ptr<texture2D> p_texture)
    : vertices(p_vertbuf)
    , m_texture(p_texture)
{
    auto& win = engine::instance()->window;
    scale(1, win.ratio);
    m_size = texture()->size();
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

vec2f
shape::screen2ndc(const vec2f coords)
{
    auto&& win = engine::instance()->window;
    return vec2f{ 2 * coords.x / win.size.x,
                  2 * coords.y / win.size.y / win.ratio };
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

const std::shared_ptr<texture2D>
shape::texture() const
{
    return m_texture;
}

void
shape::texture(std::shared_ptr<texture2D> tex)
{
    m_texture = tex;

    auto&& win = nano::engine::instance()->window;
    vec2f tex2win_ratio = texture()->size() / win.size / vec2f{ 1, win.ratio };
    // clang-format off
    vertices = { primitive_t::triangle_strip,
                 { { .pos = { tex2win_ratio.x, 0 }, .tpos = { 1, 1 } },
                   { .pos = { 0, 0 },               .tpos = { 0, 1 } },
                   { .pos = tex2win_ratio,          .tpos = { 1, 0 } },
                   { .pos = { 0, tex2win_ratio.y }, .tpos = { 0, 0 } } } };
    // clang-format on
    m_size = texture()->size();
}

const transform2D&
shape::transform() const
{
    if (transform_need_update)
    {
        transform2D new_tr;
        new_tr.rotate(rotation(), origin())
            .move(screen2ndc(position()))
            .scale(factor());
        m_transform = new_tr;

        transform_need_update = false;
    }
    return m_transform;
}

void
shape::transform(const transform2D& t)
{
    m_transform = t;
    transform_need_update = false;
}

void
shape::size(const vec2f new_size)
{
    auto&& win = engine::instance()->window;
    m_size = new_size;
    vec2f tex2win_ratio =
        size() * vec2f{ 2, 2 } / win.size / vec2f{ 1, win.ratio };
    // clang-format off
    vertices = { primitive_t::triangle_strip,
                 { { .pos = { tex2win_ratio.x, 0 }, .tpos = { 1, 1 } },
                   { .pos = { 0, 0 },               .tpos = { 0, 1 } },
                   { .pos = tex2win_ratio,          .tpos = { 1, 0 } },
                   { .pos = { 0, tex2win_ratio.y }, .tpos = { 0, 0 } } } };
    // clang-format on
}

vec2f
shape::size()
{
    return m_size;
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
shape::draw(state s) const
{
    s.transform.combine(transform());
    if (s.program)
    {
        s.program->uniform("u_texture", texture());
        s.program->uniform("u_matrix", s.transform);
        shader::use(*s.program);
    }
    else
    {
        LOG_DEBUG("Shader need for draw sprite");
        return;
    };
    vertices.draw();
}

} // namespace nano
