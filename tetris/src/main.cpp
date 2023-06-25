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

    auto tex = std::make_shared<texture2D>();
    tex->load("../assets/blue-J.png");
    nano::sprite block(tex);

    // auto time_point = std::chrono::system_clock::now();
    // auto menu = std::make_shared<tetris::menu_scene>();
    // e->scenarist.push(menu);
    //

    auto program = std::make_shared<shader>();
    err_code = shaders::transform_texture(program);
    ASSERT_ERROR(err_code, "Fail while creating default shader");

    float x{}, y{};

    bool is_running{ true };
    while (is_running)
    {
        event ev;
        while (poll_event(&ev))
        {
            using ev_t = event::type;
            switch (ev.t)
            {
            case ev_t::window_close_request:
                is_running = false;
                break;

            default:
                break;
            }
        }

        // auto cur_time_point = std::chrono::system_clock::now();
        // e->scenarist.top()->process(cur_time_point - time_point);
        // time_point = cur_time_point;

        // e->scenarist.top()->draw();

        engine::instance()->new_frame();
        ImGui::Begin("fun");

        float height = e->window.size.y / e->window.size.x;
        ImGui::SliderFloat("move x", &x, -1, 1);
        ImGui::SliderFloat("move y", &y, -height, height);

        transform2D t;
        block.position({ x, y });

        block.draw({ program, {}, t });

        ImGui::End();
        engine::instance()->renderUI();
        engine::instance()->swap_buffers();
    }
    return EXIT_SUCCESS;
}
