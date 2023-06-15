#include <nano/engine.hpp>
#include <nano/error.hpp>
#include <nano/shader.hpp>
#include <nano/sprite.hpp>
#include <nano/texture.hpp>

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
        (err) = EXIT_FAILURE;                                                  \
    }

int
main()
{
    using namespace nano;
    engine& eng = engine::instance();
    eng.initialize(engine::flag::events | engine::flag::video);

    texture2D tex;
    int err_code = tex.load("../tests/nano/transform/leo.ppm");
    // int err_code = tex.load("./test.ppm");
    TEST_ERROR(err_code, "Fail while loading texture");
    sprite s(tex);

    shader program;
    err_code = shaders::transform_texture(program, s.texture());
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
                    s.move(0, 0.01);
                    break;

                case keycode_t::kb_a:
                    s.move(-0.01, 0);
                    break;

                case keycode_t::kb_s:
                    s.move(0, -0.01);
                    break;

                case keycode_t::kb_d:
                    s.move(0.01, 0);
                    break;
                case keycode_t::kb_r:
                    is_rotate = not is_rotate;
                    break;

                case keycode_t::kb_h:
                    s.rotate(-0.1);
                    break;

                case keycode_t::kb_l:
                    s.rotate(0.1);
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
            s.rotate(time / 1e20);
        }

        program.uniform("u_matrix", s.transform());
        render(s);
        eng.swap_buffers();
    }

    eng.finalize();
    return EXIT_SUCCESS;
}
