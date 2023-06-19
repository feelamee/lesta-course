#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include <nano/shader.hpp>
#include <nano/texture2D.hpp>
#include <nano/transform2D.hpp>

namespace nano
{

struct drawable
{
    struct state
    {
        shader* program;
        texture2D* texture;
        transform2D transform;
    };

    virtual void draw(const state&) = 0;
};

} // namespace nano

#endif // DRAWABLE_HPP
