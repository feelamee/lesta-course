#include <cmath>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <numbers>
#include <numeric>

#include <canvas.hpp>
#include <color.hpp>
#include <image_loader.hpp>
#include <triangle_render.hpp>
#include <vertex.hpp>

void
lets_fun();
int
dump_load_compare();
void
draw_lines();
void
draw_triangle();
void
draw_triangle_with_indices();
void
rasterize_triangle();
void
rasterize_triangles();
void
rasterize_indexed_triangles();
void
black_white_triangle();

int
main()
{
    try
    {
        lets_fun();
        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void
lets_fun()
{
    if (dump_load_compare() == EXIT_FAILURE)
        std::cout << "dump_load_compare FAIL" << std::endl;

    draw_lines();
    draw_triangle();
    draw_triangle_with_indices();
    rasterize_triangle();
    rasterize_triangles();
    rasterize_indexed_triangles();
    black_white_triangle();
}

int
dump_load_compare()
{
    std::fstream src("04-0-input-images/canvas-dump-load-test-in.ppm");
    std::fstream dst("04-0-output-images/canvas-dump-load-test-out.ppm",
                     std::ios::out | std::ios::in);

    canvas img1;
    int ret = ppm::load(src, img1);
    if (ret == EXIT_FAILURE)
    {
        std::cerr << "file1 read error" << std::endl;
        return EXIT_FAILURE;
    }

    ret = ppm::dump<ppm::fmt::P6>(dst, img1);
    if (ret == EXIT_FAILURE)
    {
        std::cerr << "file write error" << std::endl;
        return EXIT_FAILURE;
    }

    dst.seekg(0);
    canvas img2;
    ret = ppm::load(dst, img2);
    if (ret == EXIT_FAILURE)
    {
        std::cerr << "file2 read error" << std::endl;
        return EXIT_FAILURE;
    }

    return img1 != img2;
}

void
draw_lines()
{
    std::fstream dst("04-0-output-images/line-render.ppm", std::ios::out);

    constexpr size_t w = 480, h = 270;
    canvas img(w, h);
    line_render render(img);
    render.clear();

    for (size_t i = 0; i < 300; ++i)
    {
        position start{ static_cast<position_t>(rand() % img.width()),
                        static_cast<position_t>(rand() % img.height()) };
        position end{ static_cast<position_t>(rand() % img.width()),
                      static_cast<position_t>(rand() % img.height()) };

        render.draw(start,
                    end,
                    { static_cast<uint8_t>(rand() % 256),
                      static_cast<uint8_t>(rand() % 256),
                      static_cast<uint8_t>(rand() % 256) });
    }

    ppm::dump<ppm::fmt::P3>(dst, img);
}

void
draw_triangle()
{
    std::fstream dst("04-0-output-images/triangle-render.ppm", std::ios::out);
    constexpr size_t w = 1080, h = 1080;

    canvas img(w, h);
    triangle_render render(img);
    render.clear();

    std::vector<position> triangles;
    const position center = { w / 2, h / 2 };
    size_t radius_max = std::min(w, h) / 2;

    using std::numbers::pi;
    float angle_max = pi * 15;
    float angle_step = pi / 16;
    size_t radius = radius_max * 9 / 10;
    int radius_step = 2;

    assert(angle_max / angle_step < (float)radius / radius_step);

    for (float angle{ 0 }; angle < angle_max; angle += angle_step)
    {
        radius -= radius_step;
        triangles.push_back(
            { static_cast<position_t>(center.x + radius * cos(angle)),
              static_cast<position_t>(center.y + radius * sin(angle)) });

        triangles.push_back(
            { static_cast<position_t>(center.x + radius * cos(angle + pi / 6)),
              static_cast<position_t>(center.y +
                                      radius * sin(angle + pi / 6)) });

        triangles.push_back(
            { static_cast<position_t>(center.x + radius * cos(angle + pi / 4)),
              static_cast<position_t>(center.y +
                                      radius * sin(angle + pi / 4)) });
    }

    render.draw(triangles, { 0, 0, 255 });
    ppm::dump<ppm::fmt::P6>(dst, img);
}

void
draw_triangle_with_indices()
{
    std::fstream dst("04-0-output-images/triangle-indexed-render.ppm",
                     std::ios::out);
    constexpr size_t w = 640, h = 360;
    constexpr color c = { 0, 20, 240 };

    canvas img(w, h);
    triangle_render render(img);

    std::vector<position> vertexes = {
        { 10, 10 },  { 600, 10 }, { 600, 300 },
        { 10, 300 }, { 10, 10 },  { 600, 300 },
    };
    std::vector<size_t> indexes = { 0, 1, 4, 2, 3, 5 };
    render.clear();
    render.draw(vertexes, indexes, c);
    ppm::dump<ppm::fmt::P6>(dst, img);
}

void
rasterize_triangle()
{
    std::fstream dst("04-0-output-images/triangle-rasterize.ppm",
                     std::ios::out);
    constexpr size_t w = 640, h = 360;

    canvas img(w, h);
    triangle_render render(img);
    render.clear();

    std::vector<vertex> vertices = {
        { 10, 10, 1, 0, 0 },
        { 600, 150, 0, 1, 0 },
        { 200, 300, 0, 0, 1 },
    };

    render.rasterize(vertices);
    ppm::dump<ppm::fmt::P6>(dst, img);
}

void
rasterize_triangles()
{
    std::fstream dst("04-0-output-images/triangles-rasterize-2.ppm",
                     std::ios::out);
    constexpr size_t w = 1080, h = 1080;

    canvas img(w, h);
    triangle_render render(img);
    render.clear();

    std::vector<vertex> triangles;
    const vertex center{ w / 2., h / 2. };
    size_t radius_max = std::min(w, h) / 2;
    float r{ 0.0 }, g{ 0.0 }, b{ 0.0 };

    using std::numbers::pi;
    constexpr auto max_color_val =
        std::numeric_limits<color_channel_t<>>::max();

    float angle_max = pi * 12;
    float angle_step = pi / 16;
    size_t radius = radius_max * 9 / 10;
    int radius_step = 2;

    triangles.reserve(angle_max / angle_step * 3 + 1);
    assert(angle_max / angle_step < (float)radius / radius_step);

    for (float angle{ 0 }; angle < angle_max; angle += angle_step)
    {
        radius -= radius_step;

        triangles.push_back({ (center.x + radius * cos(angle)),
                              (center.y + radius * sin(angle)),
                              r,
                              g,
                              b });

        triangles.push_back({ (center.x + radius * cos(angle + pi / 6)),
                              (center.y + radius * sin(angle + pi / 6)),
                              r,
                              g,
                              b });

        triangles.push_back({ (center.x + radius * cos(angle + pi / 2)),
                              (center.y + radius * sin(angle + pi / 2)),
                              r,
                              g,
                              b });
        r += 1. / max_color_val;
        g += r / max_color_val;
        b += g / max_color_val;
        if (b > 1)
        {
            r = 0.;
            g = 0.;
            b = 0.;
        }
    }

    render.rasterize(triangles);
    ppm::dump<ppm::fmt::P6>(dst, img);
}

void
rasterize_indexed_triangles()
{
    std::fstream dst("04-0-output-images/indexed-triangle-rasterize.ppm",
                     std::ios::out);

    constexpr size_t w = 640, h = 360;
    canvas img(w, h);
    triangle_render render(img);
    render.clear();

    std::vector<vertex> vertices = {
        { 40, 40, 1, 0., 0. },    { 600, 40, 0., 1., 0 },
        { 300, 150, 0., 0., 1. }, { 40, 320, 1., 0., 0. },
        { 600, 320, 0., 1., 0. }, { 300, 150, 0., 0., 1. }
    };

    std::vector<size_t> indices = { 0, 3, 2, 1, 4, 5 };

    render.rasterize(vertices, indices);
    ppm::dump<ppm::fmt::P6>(dst, img);
}

void
black_white_triangle()
{
    std::fstream dst("04-0-output-images/black-white-triangle.ppm",
                     std::ios::out);

    constexpr size_t w = 640, h = 360;
    canvas img(w, h);
    triangle_render render(img);
    render.program = std::make_shared<blackwhite>();
    std::dynamic_pointer_cast<blackwhite>(render.program)->uniform.buf = &img;

    const std::vector<vertex> vertices = { { 40, 40, .6, .2, .6, 0, 0 },
                                           { 600, 300, .6, .1, .0, 1., 1. },
                                           { 600, 40, 0., 0., 0., 0., 0. } };

    const std::vector<size_t> indices = { 0, 2, 1 };

    render.rasterize(vertices, indices);
    ppm::dump<ppm::fmt::P6>(dst, img);
}
