#ifndef SCOPE_GUARD_HPP
#define SCOPE_GUARD_HPP

template <typename Tag, typename Res>
class scope_guard
{
public:
    scope_guard(Res&& res)
    {
    }

    scope_guard& operator=(const scope_guard<Tag, Res>&) = delete;
    scope_guard(const scope_guard<Tag, Res>&) = delete;
};

#endif // SCOPE_GUARD_HPP
