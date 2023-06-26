#include "menu_scene.hpp"
#include "game_scene.hpp"

#include <nano/error.hpp>
#include <nano/utils.hpp>

namespace tetris
{

menu_scene::menu_scene(bool& p_is_running)
    : is_running(p_is_running)
    , node()
{
}

void
menu_scene::start()
{
    auto&& e = nano::engine::instance();

    namespace fs = std::filesystem;
    const fs::path assets_dir{ e->assets_path() };
    const fs::path bg_fn{ assets_dir / "bg.png" };

    int err_code = bg.load(bg_fn);
    if (EXIT_SUCCESS != err_code)
    {
        LOG_DEBUG("Fail loading texture");
        return;
    }

    const fs::path bg_music_fn{ assets_dir / "piano-moment.wav" };
    err_code = bg_music.load(bg_music_fn);
    if (EXIT_SUCCESS != err_code)
    {
        LOG_DEBUG("Fail loading sound");
        return;
    }

    bg_music.play();
    bg_music.volume(48);
    bg_music.loop = true;

    const fs::path bold_fp = assets_dir / "JetBrainsMonoNerdFont-Bold.ttf";
    font_bold = nano::load_font_from_file_ttf(bold_fp, 100);

    const fs::path light_fp = assets_dir / "JetBrainsMonoNerdFont-Light.ttf";
    font_light = nano::load_font_from_file_ttf(light_fp, 100);

    nano::event ev;
    ev.type = nano::event::type_t::quit;
    e->supplier.subscribe({ ev, id }, [this]() { this->is_running = false; });

    ev.type = nano::event::type_t::window_close_request;
    e->supplier.subscribe({ ev, id }, [this]() { this->is_running = false; });
}

void
menu_scene::process(delta_t delta)
{
    auto&& e = nano::engine::instance();

    ImGui::SetNextWindowPos({ 0, 0 });
    ImGui::SetNextWindowSize({ nano::engine::instance()->window.size.x,
                               nano::engine::instance()->window.size.y });
    ImGui::PushStyleColor(ImGuiCol_WindowBg, nano::color::hex({ 49, 50, 125 }));
    ImGui::PushFont(font_bold);

    ImGui::Begin("Main menu", nullptr, ImGuiWindowFlags_NoDecoration);

    ImGui::SetCursorPos({ 0, 0 });
    ImGui::Image(reinterpret_cast<ImTextureID>(bg.handle()),
                 ImGui::GetWindowSize());

    {
        ImGui::SetCursorPosX(0);
        float size = ImGui::CalcTextSize("TETRIS").x;

        float off = (ImGui::GetWindowWidth() - size) / 2;
        if (off > 0)
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
    }
    ImGui::SetCursorPosY(100);
    ImGui::Text("TETRIS");

    ImGui::PushStyleColor(ImGuiCol_Button, nano::color::hex({ 49, 50, 150 }));
    const float start_button_size = ImGui::CalcTextSize("Start").x + 100;
    {
        ImGui::SetCursorPosX(0);

        const float off = (ImGui::GetWindowWidth() - start_button_size) / 2;
        if (off > 0)
            ImGui::SetCursorPos({ off, ImGui::GetWindowHeight() * 3 / 8 });
    }
    if (ImGui::Button("Start", { start_button_size, 100 }))
    {
        auto game = std::make_shared<game_scene>(e->window.size.x, is_running);
        e->scenarist.push(game);
    }

    const float settings_button_size = ImGui::CalcTextSize("Settings").x + 100;
    {
        ImGui::SetCursorPosX(0);

        const float off = (ImGui::GetWindowWidth() - settings_button_size) / 2;
        if (off > 0)
            ImGui::SetCursorPos({ off, ImGui::GetWindowHeight() / 2 });
    }

    if (ImGui::Button("Settings", { settings_button_size, 100 }))
    {
        // settings
    }

    ImGui::PopFont();
    ImGui::PopStyleColor();

    ImGui::End();
    ImGui::PopStyleColor();
}

void
menu_scene::resume()
{
    bg_music.play();
}

void
menu_scene::pause()
{
    bg_music.pause();
}

void
menu_scene::stop()
{
    bg_music.stop();
}

void
menu_scene::draw(nano::drawable::state) const
{
}

} // namespace tetris
