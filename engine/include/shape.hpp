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
    void set_viewport(const rect<int>&);

    inline const auto data() const;
    inline auto data();

    primitive_t primitive_type() const;
    std::size_t points_count() const;
    const texture* get_texture() const;
    texture* get_texture();

    inline void move(const vec2f& offset);
    inline void scale(const vec2f& scale);
    using radian = float;
    inline void rotate(radian angle);

private:
    vertbuf vertices;
    transform m_transform;
    texture m_texture;
    rect<int> viewport;
    color fill{ colors::white };
};

void render(const shape&);

} // namespace nano

#endif // SHAPE_HPP
