#include "vertex.hpp"
#include "color.hpp"
#include <limits>

vertex::operator position() const
{
    return { .x = static_cast<position_t>(std::round(this->x)),
             .y = static_cast<position_t>(std::round(this->y)) };
}

vertex::operator color() const
{
    constexpr auto max = std::numeric_limits<color_channel_t<>>::max();
    return { .r = static_cast<color_channel_t<>>(std::round(max * this->r)),
             .g = static_cast<color_channel_t<>>(std::round(max * this->g)),
             .b = static_cast<color_channel_t<>>(std::round(max * this->b)) };
}

position::operator vertex()
{
    return { .x = static_cast<coordinate_t>(std::round(this->x)),
             .y = static_cast<coordinate_t>(round(this->y)) };
}
