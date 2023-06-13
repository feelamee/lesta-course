#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <nano/color.hpp>
#include <nano/rect.hpp>
#include <nano/shader.hpp>
#include <nano/texture.hpp>
#include <nano/transform.hpp>
#include <nano/vertbuf.hpp>

namespace nano
{

class shape
{
public:
    shape(const vertbuf&, const texture2D&);

    const vertex* data() const;
    vertex* data();

    primitive_t primitive_type() const;
    std::size_t points_count() const;
    const texture2D& texture() const;
    const transform2D& transform() const;

    void move(const vec2f& offset);
    void scale(const vec2f& scale);

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

private:
    vertbuf vertices;
    vec2f m_origin{ 0, 0 };
    vec2f m_position{ 0, 0 };
    radian m_rotation{ 0 };
    vec2f m_factor{ 1., 1. };
    mutable transform2D m_transform{};
    const texture2D& m_texture;
    color fill{ colors::white };
    mutable bool transform_need_update{ false };
};

void render(const shape&);

} // namespace nano

#endif // SHAPE_HPP
