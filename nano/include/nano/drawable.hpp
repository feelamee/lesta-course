#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include <nano/shader.hpp>
#include <nano/texture2D.hpp>
#include <nano/transform2D.hpp>
#include <optional>

namespace nano
{

struct drawable
{
    struct state
    {
        shader* program{ nullptr };
        texture2D* texture{ nullptr };
        transform2D transform;
    };

    virtual void draw(const state&) const = 0;
};

} // namespace nano

#endif // DRAWABLE_HPP
