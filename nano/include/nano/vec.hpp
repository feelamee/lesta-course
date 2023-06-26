#ifndef VEC_HPP
#define VEC_HPP

#include <assert.h>
#include <cmath>
#include <concepts>
#include <functional>
#include <variant>
// #include <ranges>

namespace nano
{

template <typename T>
    requires std::is_arithmetic_v<T>
struct vec2
{
    using type = T;
    T x{};
    T y{};

    template <typename K>
    auto
    operator+(const vec2<K>& rhs) const
    {
        return vec2{ x + rhs.x, y + rhs.y };
    }

    vec2<float>
    operator*(const vec2<float>& rhs) const
    {
        return { x * rhs.x, y * rhs.y };
    }

    template <typename K>
    vec2<float>
    operator/(const vec2<K> other) const
    {
        return { static_cast<float>(x) / other.x,
                 static_cast<float>(y) / other.y };
    }

    template <typename K>
    vec2<float>
    operator/(const K other) const
    {
        return { static_cast<float>(x) / other, static_cast<float>(y) / other };
    }

    template <typename K>
    vec2<float>
    operator*(const K other) const
    {
        return { static_cast<float>(x) * other, static_cast<float>(y) * other };
    }

    template <typename K>
    vec2<T>&
    operator+=(const vec2<K>& other)
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

    operator vec2<float>();
    operator vec2<int>();

    vec2<T>
    operator-() const
    {
        return { -x, -y };
    }

    friend bool
    operator==(const vec2<T> lhs, const vec2<T> rhs)
    {
        return std::equal_to<T>()(lhs.x, rhs.x) and
               std::equal_to<T>()(lhs.y, rhs.y);
    }

    vec2<float>
    normalized() const
    {
        float xy_sqrt = std::sqrt(x * x + y * y);
        return { x / xy_sqrt, y / xy_sqrt };
    }
};

using vec2f = vec2<float>;
using vec2i = vec2<int>;
using vec2s = vec2<std::size_t>;

template <>
inline vec2<std::size_t>::operator vec2<float>()
{
    return { static_cast<float>(x), static_cast<float>(y) };
}

template <>
inline vec2<int>::operator vec2<float>()
{
    return { static_cast<float>(x), static_cast<float>(y) };
}

inline float
strange_round(float v, float precision = 0.001)
{
    return std::round(v + precision);
}

template <>
inline vec2<float>::operator vec2<int>()
{
    return { static_cast<int>(strange_round(x)),
             static_cast<int>(strange_round(y)) };
}

template <typename T>
    requires std::is_arithmetic_v<T>
struct vec3
{
    using type = T;
    T x{};
    T y{};
    T z{};

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
