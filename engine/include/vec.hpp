#ifndef VEC_HPP
#define VEC_HPP

#include <ranges>

namespace nano
{

struct vertex;

template <typename T>
    requires std::is_arithmetic_v<T>
struct vec2
{
    using type = T;
    union
    {
        struct
        {
            T fst;
            T snd;
        };
        T vec[2];
    };

    // operator vertex();
};
using vec2f = vec2<float>;
using vec2i = vec2<int>;

template <typename T>
    requires std::is_arithmetic_v<T>
struct vec3
{
    using type = T;
    union
    {
        struct
        {
            T fst;
            T snd;
            T thd;
        };
        T vec[3];
    };
};
using vec3f = vec3<float>;

// TODO: RENAME function
template <std::ranges::input_range T, typename Proj>
    requires std::is_copy_constructible_v<T>
constexpr auto mult(const T& lhs, const T& rhs, Proj func) -> T;

} // namespace nano

#endif // VEC_HPP
