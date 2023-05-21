#ifndef VEC_HPP
#define VEC_HPP

#include <assert.h>
#include <ranges>

namespace nano
{

struct vertex;

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
};
using vec3f = vec3<float>;

// TODO: RENAME function
template <std::ranges::input_range T, typename Proj>
    requires std::is_copy_constructible_v<T>
constexpr auto
mult(const T& lhs, const T& rhs, Proj func) -> T
{
    using std::ranges::begin;
    using std::ranges::distance;
    using std::ranges::end;

    T ret{ lhs };
    auto lhs_size = distance(begin(lhs), end(lhs));
    auto rhs_size = distance(begin(rhs), end(rhs));
    assert(lhs_size == rhs_size and "size of vectors should be equal");

    auto lhs_it = begin(lhs);
    auto rhs_it = begin(rhs);

    for (auto& i : ret)
    {
        i = func((*lhs_it), (*rhs_it));
        ++lhs_it;
        ++rhs_it;
    }
    return ret;
}

} // namespace nano

#endif // VEC_HPP
