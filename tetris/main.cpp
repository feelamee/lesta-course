#include <nano/engine.hpp>
#include <nano/error.hpp>
#include <nano/sound.hpp>

#include <imgui.h>

#include <cstdlib>

int
main()
{
    using namespace nano;
    auto& e = engine::instance();
    int err_code = e.initialize(engine::flag::all);
    ASSERT_ERROR(err_code, "Fail while initialization of engine\n");

    // const std::filesystem::path bg_fn{ "../tetris/bg.png" };
    // texture2D bg;
    // err_code = bg.load(bg_fn);
    // ASSERT_ERROR(err_code, "Fail loading texture\n");

    // const std::filesystem::path bg_beat_fn{ "../tetris/piano-moment.wav" };
    // sound bg_beat;
    // err_code = bg_beat.load(bg_beat_fn);
    // ASSERT_ERROR(err_code, "Fail loading sound\n");

    // bg_beat.play();
    // bg_beat.loop = true;

    bool is_running{ true };
    while (is_running)
    {
        // event ev;
        // while (poll_event(&ev))
        // {
        //     using ev_t = event::type;
        //     switch (ev.t)
        //     {
        //     case ev_t::window_close_request:
        //         is_running = false;
        //         break;

        //     default:
        //         break;
        //     }
        // }

        // e.new_frame();

        // ImGui::SetNextWindowPos({ 0, 0 });
        // ImGui::SetNextWindowSize({ e.window.size.x, e.window.size.y });
        // ImGui::PushStyleColor(ImGuiCol_WindowBg, color::hex({ 50, 50, 125
        // }));

        // ImGui::Begin("what?", nullptr, ImGuiWindowFlags_NoDecoration);

        // ImGui::SetCursorPos({ 0, 0 });
        // ImGui::Image(reinterpret_cast<ImTextureID>(bg.handle()),
        //              { (float)bg.width(), (float)bg.height() },
        //              { 0, 0 },
        //              { bg.width() / ImGui::GetWindowWidth(),
        //                bg.height() / ImGui::GetWindowHeight() });

        // {
        //     ImGui::SetCursorPosX(0);
        //     ImGuiStyle& style = ImGui::GetStyle();

        //     float size = ImGui::CalcTextSize("TETRIS").x;

        //     float off = (ImGui::GetWindowWidth() - size) / 2;
        //     if (off > 0.0f)
        //         ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
        // }
        // ImGui::SetCursorPosY(200);
        // ImGui::SetWindowFontScale(4);
        // ImGui::Text("TETRIS");

        // ImGui::PushStyleColor(ImGuiCol_Button, color::hex({ 50, 50, 150 }));
        // ImGui::SetCursorPos({ 100, 350 });
        // ImGui::Button("Start");

        // ImGui::SetCursorPos({ 430, 350 });
        // ImGui::Button("Settings");
        // ImGui::PopStyleColor();

        // ImGui::End();
        // ImGui::PopStyleColor();

        // e.renderUI();
        e.swap_buffers();
    }

    e.finalize();
    return EXIT_SUCCESS;
}
