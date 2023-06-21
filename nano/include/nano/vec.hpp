#ifndef VEC_HPP
#define VEC_HPP

#include <assert.h>
#include <cmath>
#include <concepts>
// #include <ranges>

namespace nano
{

template <typename T>
    requires std::is_arithmetic_v<T>
struct vec2
{
    using type = T;
    T x;
    T y;

    friend vec2<T>
    operator+(const vec2<T>& lhs, const vec2<T>& rhs)
    {
        return { lhs.x + rhs.x, lhs.y + rhs.y };
    }

    vec2<T>&
    operator+=(const vec2<T>& other)
    {
        *this = *this + other;
        return *this;
    }

    vec2<T>&
    operator=(const vec2<T>& other)
    {
        x = other.x;
        y = other.y;
        return *this;
    }

    vec2<float>
    normalized()
    {
        float xy_sqrt = std::sqrt(x * x + y * y);
        return { x / xy_sqrt, y / xy_sqrt };
    }
};
using vec2f = vec2<float>;
using vec2i = vec2<int>;
using vec2s = vec2<std::size_t>;

template <typename T>
    requires std::is_arithmetic_v<T>
struct vec3
{
    using type = T;
    T x;
    T y;
    T z;

    vec3<T>
    normalized()
    {
        T xyz_sqrt = std::sqrt(x * x + y * y + z * z);
        return { x / xyz_sqrt, y / xyz_sqrt, z / xyz_sqrt };
    }
};
using vec3f = vec3<float>;

// TODO: RENAME function
// template <std::ranges::input_range T, typename Proj>
//     requires std::is_copy_constructible_v<T>
// constexpr auto
// mult(const T& lhs, const T& rhs, Proj func) -> T
// {
//     namespace rng = std::ranges;

//     T ret{ lhs };
//     const auto lhs_size = rng::distance(begin(lhs), end(lhs));
//     const auto rhs_size = rng::distance(begin(rhs), end(rhs));
//     assert(lhs_size == rhs_size and "size of vectors should be equal");

//     auto lhs_it = rng::begin(lhs);
//     auto rhs_it = rng::begin(rhs);

//     for (auto& i : ret)
//     {
//         i = func((*lhs_it), (*rhs_it));
//         ++lhs_it;
//         ++rhs_it;
//     }
//     return ret;
// }

} // namespace nano

#endif // VEC_HPP
