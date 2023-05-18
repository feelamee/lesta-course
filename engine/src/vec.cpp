#include <vec.hpp>
#include <vertex.hpp>

#include <ranges>

namespace nano
{
struct vertex;

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
    constexpr auto lhs_size = distance(begin(lhs), end(lhs));
    constexpr auto rhs_size = distance(begin(rhs), end(rhs));
    static_assert(lhs_size == rhs_size);

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

// template <>
// vec2i::operator vertex()
// {
//     vertex ret;
//     using pos_t = decltype(ret.pos)::type;
//     ret.pos = { static_cast<pos_t>(this->fst), static_cast<pos_t>(this->snd)
//     }; return ret;
// }

} // namespace nano
