#include "nano/drawable.hpp"
#include <memory>
#include <nano/engine.hpp>
#include <nano/error.hpp>
#include <nano/shader.hpp>
#include <nano/sprite.hpp>
#include <nano/texture2D.hpp>

#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

int
main()
{
    using namespace nano;
    auto&& eng = engine::instance();
    eng->initialize(engine::flag::events | engine::flag::video);

    auto tex = std::make_shared<texture2D>();
    int err_code = tex->load("../tests/nano/transform/leo.ppm");
    ASSERT_ERROR(err_code, "Fail while loading texture");
    sprite s(tex);

    auto program = std::make_shared<shader>();
    err_code = shaders::transform_texture(program);
    ASSERT_ERROR(err_code, "Fail while setting up shader program");

    bool is_rotate{ false };
    bool is_running{ true };
    bool is_rotated{ false }; /// save state when mouse focus lost
    while (is_running)
    {
        event ev;
        while (poll_event(&ev))
        {
            switch (ev.type)
            {
            case event::type_t::window_focus_lost:
                is_rotated = is_rotate;
                is_rotate = false;
                break;

            case event::type_t::window_focus_gain:
                is_rotate = is_rotated;
                break;

            case event::type_t::window_close_request:
                is_running = false;
                break;

            case event::type_t::key_down:
                switch (ev.kb.key.keycode)
                {
                case keycode_t::kb_w:
                    s.move(0, 5);
                    break;

                case keycode_t::kb_a:
                    s.move(-5, 0);
                    break;

                case keycode_t::kb_s:
                    s.move(0, -5);
                    break;

                case keycode_t::kb_d:
                    s.move(5, 0);
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

        s.draw({ program });
        eng->swap_buffers();
    }

    return EXIT_SUCCESS;
}
