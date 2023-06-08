#include <nano/engine.hpp>
#include <nano/error.hpp>
#include <nano/event.hpp>
#include <nano/resource_loader.hpp>
#include <nano/shader.hpp>
#include <nano/shape.hpp>
#include <nano/texture.hpp>
#include <nano/vec.hpp>
#include <nano/vertbuf.hpp>
#include <nano/vertex.hpp>

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
    engine& eng = engine_instance();
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

    texture2D tex;
    tex.load("../tests/nano/transform/leo.ppm");

    shape tmp(buf, tex);
    tmp.scale({ 747. / 1328, 720. / 480 });

    shader program;
    int err_code = program.load("../tests/nano/transform/fragment-shader.frag",
                                "../tests/nano/transform/vertex-shader.vert");
    if (EXIT_FAILURE == err_code)
    {
        LOG_DEBUG("Shader program loading failed");
        return EXIT_FAILURE;
    }
    err_code = EXIT_FAILURE;

    err_code = shader::link(program);
    if (EXIT_FAILURE == err_code)
    {
        LOG_DEBUG("Shader program linking failed");
        return EXIT_FAILURE;
    }
    err_code = EXIT_FAILURE;

    err_code = shader::validate(program);
    if (EXIT_FAILURE == err_code)
    {
        LOG_DEBUG("Shader program validation failure");
        return EXIT_FAILURE;
    }
    err_code = EXIT_FAILURE;

    err_code = program.uniform("u_texture", tmp.get_texture());
    if (EXIT_FAILURE == err_code)
    {
        LOG_DEBUG("Setting uniform u_texture failure");
        return EXIT_FAILURE;
    }
    err_code = EXIT_FAILURE;

    err_code = shader::use(program);
    if (EXIT_FAILURE == err_code)
    {
        LOG_DEBUG("Shader program use failure");
        return EXIT_FAILURE;
    }
    err_code = EXIT_FAILURE;

    bool is_rotate{ false };
    bool is_running{ true };
    bool is_rotated{ false }; /// save state when mouse focus lost
    while (is_running)
    {
        event ev;
        while (poll_event(&ev))
        {
            switch (ev.t)
            {
            case event::type::window_focus_lost:
                is_rotated = is_rotate;
                is_rotate = false;
                break;

            case event::type::window_focus_gain:
                is_rotate = is_rotated;
                break;

            case event::type::window_close_request:
                is_running = false;
                break;

            case event::type::key_down:
                switch (ev.kb.key.keycode)
                {
                case keycode_t::kb_w:
                    tmp.move({ 0, 0.01 });
                    break;

                case keycode_t::kb_a:
                    tmp.move({ -0.01, 0 });
                    break;

                case keycode_t::kb_s:
                    tmp.move({ 0, -0.01 });
                    break;

                case keycode_t::kb_d:
                    tmp.move({ 0.01, 0 });
                    break;
                case keycode_t::kb_r:
                    is_rotate = is_rotate ? false : true;
                    break;

                case keycode_t::kb_q:
                    is_running = false;
                    break;

                default:
                    break;
                }

            default:
                break;
            }
        }

        if (is_rotate)
        {
            auto time =
                std::chrono::system_clock::now().time_since_epoch().count();
            tmp.rotate(time);
        }

        render(tmp);
        eng.swap_buffers();
        program.uniform("u_matrix", tmp.get_transform());
    }

    eng.finalize();
    return EXIT_SUCCESS;
}
