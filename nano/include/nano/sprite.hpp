#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <nano/shape.hpp>

namespace nano
{

struct sprite : public shape
{
public:
    sprite(std::shared_ptr<texture2D>);
    sprite();
};

} // namespace nano

#endif // SPRITE_HPP
