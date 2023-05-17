#ifndef RECT_HPP
#define RECT_HPP

#include <type_traits>

#include <vec.hpp>

namespace nano
{

template <typename T>
    requires std::is_arithmetic_v<T>
struct rect
{
    vec2<T> pos;
    vec2<T> size;
};

} // namespace nano

#endif // RECT_HPP
