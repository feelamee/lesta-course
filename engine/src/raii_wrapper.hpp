#ifndef RAII_WRAPPER_HPP
#define RAII_WRAPPER_HPP

#include <functional>
#include <new>
#include <tuple>
#include <type_traits>

template <typename constructor_ret_t,
          typename destructor_ret_t,
          typename... destructor_args_t>
struct raii_wrapper
{
private:
    std::function<destructor_ret_t(destructor_args_t...)> destructor;
    std::tuple<destructor_args_t...> destructor_args_v;

    using self_type =
        raii_wrapper<constructor_ret_t, destructor_ret_t, destructor_args_t...>;

    void
    finalize()
    {
        std::apply(
            std::forward<decltype(destructor)>(destructor),
            std::forward<decltype(destructor_args_v)>(destructor_args_v));
    }

public:
    constructor_ret_t constructor_ret_v;

    constexpr raii_wrapper(
        constructor_ret_t&& p_constructor_ret_v,
        std::function<destructor_ret_t(destructor_args_t...)>&& p_destructor,
        destructor_args_t&&... p_destructor_args_v)
        : constructor_ret_v(
              std::forward<constructor_ret_t>(p_constructor_ret_v))
    {
        destructor = std::forward<decltype(p_destructor)>(p_destructor);
        destructor_args_v = std::make_tuple<destructor_args_t...>(
            std::forward<destructor_args_t>(p_destructor_args_v)...);
    }
    constexpr raii_wrapper()
    {
        destructor = []() {};
    }

    constexpr raii_wrapper(const self_type& other)
    {
        finalize();

        destructor = other.destructor;
        destructor_args_v = other.destructor_args_v;
    }

    constexpr raii_wrapper(self_type&& other) noexcept(
        (std::is_nothrow_move_assignable_v<destructor_args_t> and ...))
    {
        finalize();

        destructor = other.destructor;
        destructor_args_v = other.destructor_args_v;
    }

    constexpr self_type&
    operator=(self_type other)
    {
        finalize();

        using std::swap;
        swap(destructor, other.destructor);
        swap(constructor_ret_v, other.constructor_ret_v);
        swap(destructor_args_v, other.destructor_args_v);
        return *this;
    }

    constexpr ~raii_wrapper()
    {
        finalize();
    }
};

template <typename constructor_ret_t,
          typename destructor_ret_t,
          typename... destructor_args_t>
raii_wrapper(constructor_ret_t&& p_constructor_ret_v,
             destructor_ret_t (*p_desctructor)(destructor_args_t...),
             destructor_args_t&&... p_destructor_args_v)
    -> raii_wrapper<constructor_ret_t, destructor_ret_t, destructor_args_t...>;

#endif // RAII_WRAPPER_HPP
