#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <color.hpp>
#include <rect.hpp>
#include <shader.hpp>
#include <texture.hpp>
#include <transform.hpp>
#include <vertbuf.hpp>

namespace nano
{

class shape
{
public:
    shape(const vertbuf&, const texture&);

    void set_transform(const transform&);
    void set_texture(const texture&);

    const auto data() const;
    auto data();

    primitive_t primitive_type() const;
    std::size_t points_count() const;
    const texture* get_texture() const;
    texture* get_texture();
    const transform& get_transform() const;

    void move(const vec2f& offset);
    void scale(const vec2f& scale);
    using radian = float;
    void rotate(radian angle);

private:
    vertbuf vertices;
    vec2f origin;
    vec2f position;
    radian rotation;
    vec2f factor{ 1., 1. };
    mutable transform m_transform;
    texture m_texture;
    color fill{ colors::white };
    mutable bool transform_need_update{ true };
};

void render(const shape&);

} // namespace nano

#endif // SHAPE_HPP
