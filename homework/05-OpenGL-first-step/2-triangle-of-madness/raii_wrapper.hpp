#ifndef RAII_WRAPPER_HPP
#define RAII_WRAPPER_HPP

#include <functional>
#include <tuple>

template <typename constructor_ret_t,
          typename destructor_ret_t,
          typename... destructor_args_t>
struct raii_wrapper
{
private:
    std::function<destructor_ret_t(destructor_args_t...)> destructor;
    std::tuple<destructor_args_t...> destructor_args_v;

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
    constexpr ~raii_wrapper()
    {
        std::apply(
            std::forward<decltype(destructor)>(destructor),
            std::forward<decltype(destructor_args_v)>(destructor_args_v));
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
