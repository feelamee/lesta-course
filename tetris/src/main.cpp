#include "menu_scene.hpp"
#include "tetramino.hpp"

#include <nano/engine.hpp>
#include <nano/error.hpp>
#include <nano/scene.hpp>
#include <nano/sound.hpp>
#include <nano/utils.hpp>

#include <imgui.h>

#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>

extern "C" int
main()
{
    using namespace nano;
    auto&& e = engine::instance();
    int err_code = e->initialize(engine::flag::all);
    ASSERT_ERROR(err_code, "Fail while initialization of engine");

    auto program = std::make_shared<shader>();
    err_code = shaders::transform_texture(program);
    ASSERT_ERROR(err_code, "Fail while creating default shader");

    auto menu = std::make_shared<tetris::menu_scene>(e->window.size.x);
    e->scenarist.push(menu);

    e->start();
    while (e->is_running())
    {
        event ev;
        while (poll_event(&ev))
        {
            if (ev.type == nano::event::type_t::key_down)
            {
                LOG_DEBUG("KEY_DOWN %c", ev.kb.key.keycode);
            }
            e->supplier.deliver(ev);
        }

        e->new_frame();

        if (e->scenarist.top())
        {
            using namespace std::chrono_literals;
            e->scenarist.top()->process(16ms);
        }

        if (e->scenarist.top())
        {
            e->scenarist.top()->draw({ program });
        }

        e->render();
    }
    return EXIT_SUCCESS;
}
