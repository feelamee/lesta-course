#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <color.hpp>
#include <rect.hpp>
#include <texture.hpp>
#include <vertbuf.hpp>

namespace nano
{

class shape
{
private:
    vertbuf vertices;
    texture m_texture;
    rect<int> viewport;
    color fill{ colors::white };
};

} // namespace nano

#endif // SHAPE_HPP
