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

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

int
main()
{
    using namespace nano;
    engine& eng = engine_instance();
    eng.initialize();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();

    bool is_running = true;

    bool show_demo_window = false;
    bool show_another_window = true;
    int exit_code{ EXIT_FAILURE };

    while (is_running)
    {
        SDL_Event ev;
        while (SDL_PollEvent(&ev))
        {
            ImGui_ImplSDL3_ProcessEvent(&ev);
            if (ev.key.keysym.sym == SDLK_q)
            {
                is_running = false;
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
