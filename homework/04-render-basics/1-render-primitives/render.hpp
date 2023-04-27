#ifndef RENDER_HPP
#define RENDER_HPP

#include "color.hpp"
#include "canvas.hpp"

#include <vector>
#include <boost/numeric/ublas/matrix.hpp>
using namespace boost::numeric;

template<typename compT = uint8_t>
struct render
{
    virtual void clear(color<compT> = color_black) = 0;
    virtual canvas<compT>& data() = 0;

    using position = std::pair<size_t, size_t>;
    virtual std::vector<position> line (position, position) = 0;
    virtual void draw(position, position, color<compT>) = 0;
};

#endif // RENDER_HPP