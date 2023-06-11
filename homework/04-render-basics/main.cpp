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
#include <resource_loader.hpp>
#include <triangle_render.hpp>
#include <vertex.hpp>

void lets_fun();
int dump_load_compare();
void draw_lines();
void draw_triangle();
void draw_triangle_with_indices();
void rasterize_triangle();
void rasterize_triangles();
void rasterize_indexed_triangles();
void black_white_triangle();

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
    if (dump_load_compare() != EXIT_SUCCESS)
        std::cout << "dump_load_compare FAIL" << std::endl;
    else
        std::cout << "dump_load_compare SUCCESS" << std::endl;

    draw_lines();
    std::clog << "lines drawed" << std::endl;
    draw_triangle();
    std::clog << "draw_triangle" << std::endl;
    draw_triangle_with_indices();
    std::clog << "draw_triangle_with_indices" << std::endl;
    rasterize_triangle();
    std::clog << "rasterize_triangle" << std::endl;
    rasterize_triangles();
    std::clog << "rasterize_triangles" << std::endl;
    rasterize_indexed_triangles();
    std::clog << "rasterize_indexed_triangles" << std::endl;
    black_white_triangle();
    std::clog << "black_whie_trangle" << std::endl;
}

int
dump_load_compare()
{
    std::ifstream src("../homework/04-render-basics/leo.ppm", std::ios::binary);
    std::ofstream dst("04-0-output-images/canvas-dump-load-test-out.ppm",
                      std::ios::binary);

    canvas img1;
    img1.fill(colors::green);
    int ret = ppm::load(src, img1);

    if (ret == EXIT_FAILURE)
    {
        std::cerr << ppm::error2str(ppm::error()) << std::endl;
        return EXIT_FAILURE;
    }

    ret = ppm::dump(dst, img1, ppm::fmt::P6);
    if (ret == EXIT_FAILURE)
    {
        std::cerr << ppm::error2str(ppm::error()) << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream dst_src("04-0-output-images/canvas-dump-load-test-out.ppm",
                          std::ios::binary);
    canvas img2;
    ret = ppm::load(dst_src, img2);
    if (ret == EXIT_FAILURE)
    {
        std::cerr << ppm::error2str(ppm::error()) << std::endl;
        return EXIT_FAILURE;
    }

    return not(img1 == img2);
}

void
draw_lines()
{
    std::ofstream dst("04-0-output-images/line-render.ppm", std::ios::binary);

    constexpr size_t w = 480, h = 270;
    canvas img(w, h);
    line_render render(img);
    render.clear();

    for (size_t i = 0; i < 300; ++i)
    {
        position start{ static_cast<position::type>(rand() % img.width()),
                        static_cast<position::type>(rand() % img.height()) };
        position end{ static_cast<position::type>(rand() % img.width()),
                      static_cast<position::type>(rand() % img.height()) };

        render.draw(start,
                    end,
                    { static_cast<uint8_t>(rand() % 256),
                      static_cast<uint8_t>(rand() % 256),
                      static_cast<uint8_t>(rand() % 256) });
    }

    ppm::dump(dst, img, ppm::fmt::P3);
}

void
draw_triangle()
{
    std::ofstream dst("04-0-output-images/triangle-render.ppm",
                      std::ios::binary);
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
            { static_cast<position::type>(center.x + radius * cos(angle)),
              static_cast<position::type>(center.y + radius * sin(angle)) });

        triangles.push_back({ static_cast<position::type>(
                                  center.x + radius * cos(angle + pi / 6)),
                              static_cast<position::type>(
                                  center.y + radius * sin(angle + pi / 6)) });

        triangles.push_back({ static_cast<position::type>(
                                  center.x + radius * cos(angle + pi / 4)),
                              static_cast<position::type>(
                                  center.y + radius * sin(angle + pi / 4)) });
    }

    render.draw(triangles, { 0, 0, 255 });
    ppm::dump(dst, img, ppm::fmt::P6);
}

void
draw_triangle_with_indices()
{
    std::ofstream dst("04-0-output-images/triangle-indexed-render.ppm",
                      std::ios::binary);
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
    ppm::dump(dst, img, ppm::fmt::P6);
}

void
rasterize_triangle()
{
    std::ofstream dst("04-0-output-images/triangle-rasterize.ppm",
                      std::ios::binary);
    constexpr size_t w = 640, h = 360;

    canvas img(w, h);
    triangle_render render(img);
    render.clear();

    std::vector<vertex> vertices = {
        { 10, 10, 255, 0, 0 },
        { 600, 150, 0, 255, 0 },
        { 200, 300, 0, 0, 255 },
    };

    render.rasterize(vertices);
    ppm::dump(dst, img, ppm::fmt::P6);
}

void
rasterize_triangles()
{
    std::ofstream dst("04-0-output-images/triangles-rasterize-2.ppm",
                      std::ios::binary);
    constexpr size_t w = 1080, h = 1080;

    canvas img(w, h);
    triangle_render render(img);
    render.clear();

    std::vector<vertex> triangles;
    const vertex center{ w / 2., h / 2. };
    size_t radius_max = std::min(w, h) / 2;
    color::channel_t r{ 0 }, g{ 0 }, b{ 0 };

    using std::numbers::pi;
    constexpr auto max_color_val = std::numeric_limits<color::channel_t>::max();

    float angle_max = pi * 12;
    float angle_step = pi / 16;
    size_t radius = radius_max * 9 / 10;
    int radius_step = 2;

    triangles.reserve(angle_max / angle_step * 3 + 1);
    assert(angle_max / angle_step < (float)radius / radius_step);

    for (float angle{ 0 }; angle < angle_max; angle += angle_step)
    {
        radius -= radius_step;

        triangles.push_back(
            { static_cast<float>((center.pos.x + radius * cos(angle))),
              static_cast<float>((center.pos.y + radius * sin(angle))),
              r,
              g,
              b });

        triangles.push_back(
            { static_cast<float>((center.pos.x + radius * cos(angle + pi / 6))),
              static_cast<float>((center.pos.y + radius * sin(angle + pi / 6))),
              r,
              g,
              b });

        triangles.push_back(
            { static_cast<float>((center.pos.x + radius * cos(angle + pi / 2))),
              static_cast<float>((center.pos.y + radius * sin(angle + pi / 2))),
              r,
              g,
              b });
        r += 1;
        g += r;
        b += g;
        if (r >= max_color_val)
            r = 0;
        if (g >= max_color_val)
            g = 0;
        if (b >= max_color_val)
            b = 0;
    }

    render.rasterize(triangles);
    ppm::dump(dst, img, ppm::fmt::P6);
}

void
rasterize_indexed_triangles()
{
    std::ofstream dst("04-0-output-images/indexed-triangle-rasterize.ppm",
                      std::ios::binary);

    constexpr size_t w = 640, h = 360;
    canvas img(w, h);
    triangle_render render(img);
    render.clear();

    std::vector<vertex> vertices = {
        { 40, 40, 255, 0, 0 },   { 600, 40, 0, 255, 0 },
        { 300, 150, 0, 0, 255 }, { 40, 320, 255, 0, 0 },
        { 600, 320, 0, 255, 0 }, { 300, 150, 0, 0, 255 }
    };

    std::vector<size_t> indices = { 0, 3, 2, 1, 4, 5 };

    render.rasterize(vertices, indices);
    ppm::dump(dst, img, ppm::fmt::P6);
}

void
black_white_triangle()
{
    std::ofstream dst("04-0-output-images/black-white-triangle.ppm",
                      std::ios::binary);

    constexpr size_t w = 640, h = 360;
    canvas img(w, h);
    triangle_render render(img);
    render.program = std::make_shared<blackwhite>();
    std::dynamic_pointer_cast<blackwhite>(render.program)->uniform.buf = &img;

    const std::vector<vertex> vertices = { { 40, 40, 180, 50, 180, 0, 0 },
                                           { 600, 300, 180, 25, 0, 1., 1. },
                                           { 600, 40, 0, 0, 0, 0., 0. } };

    const std::vector<size_t> indices = { 0, 2, 1 };

    render.rasterize(vertices, indices);
    ppm::dump(dst, img, ppm::fmt::P3);
}
