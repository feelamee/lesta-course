#include <nano/sprite.hpp>

#include <nano/engine.hpp>
#include <nano/texture2D.hpp>
#include <nano/vertbuf.hpp>

namespace nano
{

sprite::sprite()
    : shape()
{
}

sprite::sprite(std::shared_ptr<texture2D> tex)
    : shape()
{
    texture(tex);
}

} // namespace nano
