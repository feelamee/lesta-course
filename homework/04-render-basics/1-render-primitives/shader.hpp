#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include "canvas.hpp"
#include "color.hpp"
#include "vertex.hpp"
#include <limits>
#include <memory>
#include <numeric>

struct shader
{
    virtual vertex
    vertex_shader(const vertex&) = 0;

    virtual color
    fragment_shader(const vertex&) = 0;
};

struct dummy : shader
{
    struct
    {
        canvas* buf = nullptr;
    } uniform;

    vertex
    vertex_shader(const vertex& v) override
    {
        return v;
    }

    color
    fragment_shader(const vertex& v) override
    {
        auto width = (*uniform.buf).width() - 1;
        auto height = (*uniform.buf).height() - 1;
        color result = (*uniform.buf)(static_cast<position_t>(width * v.tx),
                                      static_cast<position_t>(height * v.ty));
        return result;
    }
};

struct blackwhite : shader
{
    struct
    {
        canvas* buf = nullptr;
    } uniform;

    vertex
    vertex_shader(const vertex& v) override
    {
        return v;
    }

    color
    fragment_shader(const vertex& v) override
    {
        auto width = (*uniform.buf).width() - 1;
        auto height = (*uniform.buf).height() - 1;
        color result = (*uniform.buf)(width * v.tx, height * v.ty);
        auto avg = (result.r + result.g + result.b) / 3;
        result.r = avg;
        result.g = avg;
        result.b = avg;
        return result;
    }
};

#endif // SHADER_PROGRAM_HPP
