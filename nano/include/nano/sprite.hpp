#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <nano/shape.hpp>

namespace nano
{

struct sprite : public shape
{
public:
    sprite(const texture2D& tex);
};

} // namespace nano

#endif // SPRITE_HPP
