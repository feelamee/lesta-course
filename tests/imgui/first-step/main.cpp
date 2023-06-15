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

#include <cstdlib>
#include <iostream>
#include <ranges>
#include <vector>

#include <imgui.h>

// #include <imgui.h>

int
main()
{
    using namespace nano;
    engine& eng = engine::instance();
    int err_code = eng.initialize(engine::flag::events | engine::flag::video);
    if (EXIT_SUCCESS != err_code)
    {
        LOG_DEBUG("Failed while initializing engine");
        return err_code;
    }

    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();

    bool is_running = true;

    bool show_demo_window = true;
    bool show_another_window = true;
    int exit_code{ EXIT_FAILURE };

    while (is_running)
    {
        event ev;
        while (poll_event(&ev))
        {
            switch (ev.t)
            {
            case event::type::key_up:
                if (ev.kb.key.keycode == keycode_t::kb_q)
                    is_running = false;
                break;

            case event::type::window_close_request:
                is_running = false;
                break;

            default:
                break;
            }
        }

        engine::new_frame();

        if (show_demo_window)
        {
            ImGui::ShowDemoWindow(&show_demo_window);
        }

        if (show_another_window)
        {
            ImGui::Begin("Dear ImGui Window", &show_another_window);
            ImGui::Text("FPS: %f", io.Framerate);
            if (ImGui::Button("EXIT_FAILURE"))
            {
                exit_code = EXIT_FAILURE;
                is_running = false;
            }
            if (ImGui::Button("EXIT_SUCCES"))
            {
                exit_code = EXIT_SUCCESS;
                is_running = false;
            }
            ImGui::End();
        }

        engine::renderUI();
        eng.swap_buffers();
    }

    eng.finalize();
    return exit_code;
}
