#include "nano/transform.hpp"
#include <nano/engine.hpp>
#include <nano/error.hpp>
#include <nano/event.hpp>
#include <nano/resource_loader.hpp>
#include <nano/shader.hpp>
#include <nano/shape.hpp>
#include <nano/texture.hpp>
#include <nano/utils.hpp>
#include <nano/vec.hpp>
#include <nano/vertbuf.hpp>
#include <nano/vertex.hpp>

#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

#define TEST_ERROR(err, msg)                                                   \
    {                                                                          \
        if (EXIT_FAILURE == (err))                                             \
        {                                                                      \
            LOG_DEBUG((msg));                                                  \
            return EXIT_FAILURE;                                               \
        }                                                                      \
        err_code = EXIT_FAILURE;                                               \
    }

int
main()
{
    using namespace nano;
    engine& eng = engine::instance();
    eng.initialize();

    texture2D tex;
    tex.load("../tests/nano/transform/leo.ppm");
    const float tex_ratio = (float)tex.width() / tex.height();

    // clang-format off
    vertbuf buf(primitive_t::triangle_strip,
                {
                    { {  .3*tex_ratio, -.3 }, { 0, 0, 0 }, { 1, 1 } },
                    { { -.3*tex_ratio, -.3 }, { 0, 0, 0 }, { 0, 1 } },
                    { {  .3*tex_ratio,  .3 }, { 0, 0, 0 }, { 1, 0 } },
                    { { -.3*tex_ratio,  .3 }, { 0, 0, 0 }, { 0, 0 } },
                });
    // clang-format on

    shape sprite(buf, tex);
    sprite.scale({ 2, 2 * (float)eng.window.width / eng.window.height });

    shader program;
    int err_code = shaders::transform_texture(program, sprite.texture());
    TEST_ERROR(err_code, "Fail while setting up shader program");
    shader::use(program);

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
                    sprite.move({ 0, 0.01 });
                    break;

                case keycode_t::kb_a:
                    sprite.move({ -0.01, 0 });
                    break;

                case keycode_t::kb_s:
                    sprite.move({ 0, -0.01 });
                    break;

                case keycode_t::kb_d:
                    sprite.move({ 0.01, 0 });
                    break;
                case keycode_t::kb_r:
                    is_rotate = not is_rotate;
                    break;

                case keycode_t::kb_h:
                    sprite.rotate(-0.1);
                    break;

                case keycode_t::kb_l:
                    sprite.rotate(0.1);
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
            sprite.rotate(time / 1e20);
        }

        program.uniform("u_matrix", sprite.transform());
        render(sprite);
        eng.swap_buffers();
    }

    eng.finalize();
    return EXIT_SUCCESS;
}
