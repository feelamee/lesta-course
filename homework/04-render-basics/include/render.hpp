#ifndef RENDER_HPP
#define RENDER_HPP

#include "canvas.hpp"
#include "color.hpp"
#include "vertex.hpp"

#include <vector>

struct render
{
    virtual void clear(color = colors::black) = 0;
    virtual canvas& data() = 0;

    virtual std::vector<position> line(position, position) = 0;
    virtual void draw(position, position, color) = 0;
};

#endif // RENDER_HPP
