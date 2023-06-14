#include <nano/sprite.hpp>

#include <nano/engine.hpp>
#include <nano/texture.hpp>
#include <nano/vertbuf.hpp>

namespace nano
{

// clang-format off
sprite::sprite(const texture2D& tex)
    : rectangle({primitive_t::triangle_strip,
                  {{ .pos = { tex.relwidth(), 0 },  .tpos = { 1, 1 } }
                  ,{ .pos = { 0, 0 },               .tpos = { 0, 1 } }
                  ,{ .pos = tex.relsize(),          .tpos = { 1, 0 } }
                  ,{ .pos = { 0, tex.relheight() }, .tpos = { 0, 0 } }}
                },
                tex)
{
}
// clang-format on

const texture2D&
sprite::texture() const
{
    return rectangle.texture();
}

const transform2D&
sprite::transform() const
{
    return rectangle.transform();
}

void
sprite::move(const vec2f& offset)
{
    rectangle.move(offset);
}

void
sprite::scale(const vec2f& scale)
{
    rectangle.scale(scale);
}

void
sprite::rotate(radian angle)
{
    rectangle.rotate(angle);
}

void
sprite::move(vec2f::type offset_x, vec2f::type offset_y)
{
    move({ offset_x, offset_y });
}

void
sprite::scale(vec2f::type scale_x, vec2f::type scale_y)
{
    scale({ scale_x, scale_y });
}

void
render(const sprite& s)
{
    render(s.rectangle);
}

} // namespace nano
