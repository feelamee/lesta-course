#include "vertex.hpp"
#include "color.hpp"
#include <limits>

namespace nano
{

// vertex::operator vec2i() const
// {
//     return { static_cast<vec2i::type>(std::round(this->pos.fst)),
//              static_cast<vec2i::type>(std::round(this->pos.snd)) };
// }

// vertex::operator struct color() const
// {
//     constexpr auto max = std::numeric_limits<color_channel_t<>>::max();
//     return {
//         .r = static_cast<color_channel_t<>>(std::round(max *
//         this->color.fst)), .g = static_cast<color_channel_t<>>(std::round(max
//         * this->color.snd)), .b =
//         static_cast<color_channel_t<>>(std::round(max * this->color.thd))
//     };
// }

} // namespace nano
