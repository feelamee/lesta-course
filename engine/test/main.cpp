#include <chrono>
#include <cmath>
#include <event.hpp>
#include <functional>
#include <iengine.hpp>
#include <image_loader.hpp>
#include <shader.hpp>
#include <shape.hpp>
#include <texture.hpp>
#include <vec.hpp>
#include <vertbuf.hpp>
#include <vertex.hpp>

#include <concepts>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

void
print(const std::ranges::range auto& r)
{
    for (const auto& i : r)
        std::cout << i << " ";
    std::cout << std::endl;
}

template <typename T>
void
print(const T* r, std::size_t w, std::size_t h)
{
    for (auto i{ 0 }; i < w; ++i)
    {
        for (auto j{ 0 }; j < h; ++j)
            std::cout << r[i * h + j] << " ";
        std::cout << std::endl;
    }
}

int
main()
{
    using namespace nano;
    iengine& eng = engine_instance();
    eng.initialize();

    // clang-format off
    vertbuf buf(
        primitive_t::triangle_strip,
        {
          {{-.3,  .3},   {0, 0, 0},    {0, 0}},
          {{ .3,  .3},   {0, 0, 0},    {0, 1}},
          {{-.3, -.3},   {0, 0, 0},    {1, 0}},
          {{ .3, -.3},   {0, 0, 0},    {1, 1}},
        });
    // clang-format on

    std::ifstream file("../engine/test/leo.ppm");
    canvas img;
    int img_load_res = ppm::load(file, img);
    if (EXIT_FAILURE == img_load_res)
    {
        std::cerr << ppm::error2str(ppm::error()) << std::endl;
        return EXIT_FAILURE;
    }

    img.transpose();
    texture tex(img);
    shape tmp(buf, tex);
    tmp.scale({ 747. / 1328, 720. / 480 });

    auto program = setup_shaders("../engine/test/vertex-shader.vert",
                                 "../engine/test/fragment-shader.frag");
    if (-1 == program)
    {
        eng.finalize();
        std::cerr << "setup shader was failed" << std::endl;
        return EXIT_FAILURE;
    }
    eng.set_program(program);

    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;

    bool is_rotate = false;
    bool is_running = true;
    while (is_running)
    {
        event ev;
        while (poll_event(&ev))
        {
            switch (ev.type)
            {
            case event_t::key_down:
                switch (ev.key.sym.sym)
                {
                case keycode::kb_w:
                    tmp.move({ 0, 0.01 });
                    break;

                case keycode::kb_a:
                    tmp.move({ -0.01, 0 });
                    break;

                case keycode::kb_s:
                    tmp.move({ 0, -0.01 });
                    break;

                case keycode::kb_d:
                    tmp.move({ 0.01, 0 });
                    break;
                case keycode::kb_r:
                    is_rotate = is_rotate ? false : true;
                    break;
                }
                break;
            }
            if (ev.key.sym.sym == keycode::kb_q)
                is_running = false;
            break;
        }

        if (is_rotate)
        {
            auto time =
                std::chrono::system_clock::now().time_since_epoch().count();
            tmp.rotate(time);
        }

        render(tmp);
        eng.swap_buffers();
    }

    eng.finalize();
    return EXIT_SUCCESS;
}
