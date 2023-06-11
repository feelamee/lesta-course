#ifndef SHADER_HPP
#define SHADER_HPP

#include "canvas.hpp"
#include "color.hpp"
#include "vertex.hpp"
#include <cmath>
#include <limits>
#include <memory>
#include <numeric>

struct shader
{
    virtual vertex vertex_shader(const vertex&) = 0;

    virtual color fragment_shader(const vertex&) = 0;
};

struct texture_shader : shader
{
    struct
    {
        canvas* buf = nullptr;
    } uniform;

    vertex vertex_shader(const vertex& v) override;

    color fragment_shader(const vertex& v) override;
};

struct dummy : shader
{
    vertex
    vertex_shader(const vertex& v) override
    {
        return v;
    }

    color
    fragment_shader(const vertex& v) override
    {
        return v.rgb;
    }
};

struct blackwhite : shader
{
    struct
    {
        canvas* buf = nullptr;
    } uniform;

    vertex vertex_shader(const vertex& v) override;

    color fragment_shader(const vertex& v) override;
};

struct funny_moment : shader
{
    struct
    {
        float mouse_x;
        float mouse_y;
        float radius = 30;
        canvas* buf = nullptr;
    } uniform;

    vertex vertex_shader(const vertex& v) override;

    color fragment_shader(const vertex& v) override;
};

struct lupa : shader
{
    struct
    {
        float mouse_x;
        float mouse_y;
        float radius = 30;
        float scale = 0.1;
        canvas* buf = nullptr;
    } uniform;

    vertex vertex_shader(const vertex& v) override;

    color fragment_shader(const vertex& v) override;
};

struct blur : shader
{
    struct
    {
        float mouse_x;
        float mouse_y;
        float strength = 5;
        float radius = 30;
        canvas* buf = nullptr;
    } uniform;

    vertex vertex_shader(const vertex& v) override;

    color fragment_shader(const vertex& v) override;
};

#endif // SHADER_HPP
