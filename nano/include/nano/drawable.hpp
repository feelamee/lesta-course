#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include <cstddef>
#include <memory>
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
        state(std::shared_ptr<shader> p_program = nullptr,
              std::shared_ptr<texture2D> p_texture = nullptr,
              transform2D p_transform = {})
            : program(p_program)
            , texture(p_texture)
            , transform(p_transform)
        {
        }

        std::shared_ptr<shader> program;
        std::shared_ptr<texture2D> texture;
        transform2D transform;
    };

    virtual void draw(state = {}) const = 0;
};

} // namespace nano

#endif // DRAWABLE_HPP
