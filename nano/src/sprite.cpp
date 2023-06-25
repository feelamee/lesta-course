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
    // clang-format off
  : shape({primitive_t::triangle_strip,
                  {{ .pos = { tex->size().normalized().x, 0 }, .tpos = { 1, 1 } }
                  ,{ .pos = { 0, 0 },                          .tpos = { 0, 1 } }
                  ,{ .pos = tex->size().normalized(),          .tpos = { 1, 0 } }
                  ,{ .pos = { 0, tex->size().normalized().y }, .tpos = { 0, 0 } }}},
          tex)
// clang-format on
{
}

} // namespace nano
