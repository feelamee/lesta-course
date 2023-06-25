#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <nano/color.hpp>
#include <nano/drawable.hpp>
#include <nano/texture2D.hpp>
#include <nano/transform2D.hpp>
#include <nano/vertbuf.hpp>

namespace nano
{

class shape : drawable
{
public:
    shape(const vertbuf&, std::shared_ptr<texture2D>);
    shape();

    const vertex* data() const;
    vertex* data();

    primitive_t primitive_type() const;
    std::size_t points_count() const;
    const std::shared_ptr<texture2D> texture() const;
    void texture(std::shared_ptr<texture2D> tex);
    const transform2D& transform() const;
    void transform(const transform2D&);

    void move(const vec2f& offset);
    void scale(const vec2f& scale);

    void move(vec2f::type offset_x, vec2f::type offset_y);
    void scale(vec2f::type scale_x, vec2f::type scale_y);

    using radian = float;
    void rotate(radian angle);

    vec2f origin() const;
    vec2f position() const;
    radian rotation() const;
    vec2f factor() const;

    void origin(const vec2f& p_origin);
    void position(const vec2f& p_posiiton);
    void rotation(radian p_rotation);
    void factor(const vec2f& p_factor);
    void draw(const state = {}) const override;

protected:
    vertbuf vertices;
    vec2f m_origin{ 0, 0 };
    vec2f m_position{ 0, 0 };
    radian m_rotation{ 0 };
    vec2f m_factor{ 1., 1. };
    mutable transform2D m_transform{};
    std::shared_ptr<texture2D> m_texture;
    color fill{ colors::white };
    mutable bool transform_need_update{ false };
};

} // namespace nano

#endif // SHAPE_HPP
