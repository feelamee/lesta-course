#include <cstdio>
#include <nano/error.hpp>
#include <nano/event.hpp>
#include <nano/iengine.hpp>
#include <nano/image_loader.hpp>
#include <nano/shader.hpp>
#include <nano/shape.hpp>
#include <nano/texture.hpp>
#include <nano/vec.hpp>
#include <nano/vertbuf.hpp>
#include <nano/vertex.hpp>

#include <chrono>
#include <cmath>
#include <concepts>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <vector>

#include "../../../nano/src/engine.hpp"
#include "SDL_keycode.h"
#include <glad/glad.h>

#include <SDL3/SDL.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

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
    static engine eng;
    eng.initialize();
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForOpenGL(eng.window.get(), eng.context.get());
    ImGui_ImplOpenGL3_Init("#version 300 es");

    bool is_rotate = false;
    bool is_running = true;

    bool show_demo_window = false;
    bool show_another_window = true;

    while (is_running)
    {
        SDL_Event ev;
        while (SDL_PollEvent(&ev))
        {
            ImGui_ImplSDL3_ProcessEvent(&ev);
            if (ev.key.keysym.sym == SDLK_q)
                is_running = false;
            break;
        }

        ImGui_ImplSDL3_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        if (show_demo_window)
        {
            ImGui::ShowDemoWindow(&show_demo_window);
        }

        if (show_another_window)
        {
            ImGui::Begin("Dear ImGui Window", &show_another_window);
            ImGui::Text("Hello from Dear ImGui window!");
            ImGui::Checkbox("touch me", &show_another_window);
            if (ImGui::Button("close me"))
                show_another_window = false;
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        eng.swap_buffers();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    eng.finalize();
    return EXIT_SUCCESS;
}
