#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <nano/shape.hpp>

namespace nano
{

class sprite
{
public:
    sprite(const texture2D& tex);

    const texture2D& texture() const;
    const transform2D& transform() const;

    void move(const vec2f& offset);
    void scale(const vec2f& scale);

    void move(vec2f::type offset_x, vec2f::type offset_y);
    void scale(vec2f::type scale_x, vec2f::type scale_y);

    using radian = float;
    void rotate(radian angle);

    friend void render(const sprite&);

private:
    shape rectangle;
};

} // namespace nano

#endif // SPRITE_HPP
