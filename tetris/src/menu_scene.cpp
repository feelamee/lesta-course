#include "menu_scene.hpp"
#include "game_scene.hpp"

#include <nano/error.hpp>
#include <nano/utils.hpp>

namespace tetris
{

menu_scene::menu_scene()
    : node()
{
    auto&& e = nano::engine::instance();
    const std::filesystem::path assets_dir{ e->assets_path() };
    const std::filesystem::path bg_music_fn{ assets_dir / "piano-moment.wav" };

    int err_code = bg_music.load(bg_music_fn);
    if (EXIT_SUCCESS != err_code)
    {
        LOG_DEBUG("Fail loading sound");
        return;
    }

    const std::filesystem::path bold_fp =
        assets_dir / "JetBrainsMonoNerdFont-Bold.ttf";
    font_bold = nano::load_font_from_file_ttf(bold_fp, 100);

    const std::filesystem::path light_fp =
        assets_dir / "JetBrainsMonoNerdFont-Light.ttf";
    font_light = nano::load_font_from_file_ttf(light_fp, 100);

    using namespace std::placeholders;
    nano::event ev;
    ev.type = nano::event::type_t::quit;
    e->supplier.subscribe({ ev, id }, std::bind(&nano::engine::stop, e));

    ev.type = nano::event::type_t::window_close_request;
    e->supplier.subscribe({ ev, id }, std::bind(&nano::engine::stop, e));

    bg_music.volume(50);
    bg_music.loop = true;
}

void
menu_scene::start()
{
    bg_music.play();
}

void
menu_scene::process(delta_t delta)
{
    auto&& e = nano::engine::instance();

    ImGui::SetNextWindowPos({ 0, 0 });
    ImGui::SetNextWindowSize({ nano::engine::instance()->window.size.x,
                               nano::engine::instance()->window.size.y });
    ImGui::PushFont(font_bold);
    ImGui::SetNextWindowBgAlpha(0);
    ImGui::Begin("Main menu", nullptr, ImGuiWindowFlags_NoDecoration);

    ImGui::SetCursorPos({ 0, 0 });
    {
        ImGui::SetCursorPosX(0);
        float size = ImGui::CalcTextSize("TETRIS").x;

        float off = (ImGui::GetWindowWidth() - size) / 2;
        if (off > 0)
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
    }
    ImGui::SetCursorPosY(100);
    ImGui::PushStyleColor(ImGuiCol_Text, nano::color::hex({ 50, 0, 70 }));
    ImGui::Text("TETRIS");
    ImGui::PopStyleColor();

    ImGui::PushStyleColor(ImGuiCol_Button, nano::color::hex({ 49, 50, 150 }));
    const float start_button_size = e->window.size.x / 3 * 2;
    // ImGui::CalcTextSize("Start").x + 100;
    {
        ImGui::SetCursorPosX(0);

        const float off = (ImGui::GetWindowWidth() - start_button_size) / 2;
        if (off > 0)
            ImGui::SetCursorPos({ off, ImGui::GetWindowHeight() * 1 / 4 });
    }
    if (ImGui::Button("Start", { start_button_size, 200 }))
    {
        auto game = std::make_shared<game_scene>(e->window.size.x);
        e->scenarist.push(game);
    }

    const float settings_button_size = e->window.size.x / 4 * 3;
    // ImGui::CalcTextSize("Settings").x + 100;
    {
        ImGui::SetCursorPosX(0);

        const float off = (ImGui::GetWindowWidth() - settings_button_size) / 2;
        if (off > 0)
            ImGui::SetCursorPos({ off, ImGui::GetWindowHeight() / 2 });
    }

    if (ImGui::Button("Settings", { settings_button_size, 200 }))
    {
        // settings
    }

    ImGui::PopStyleColor();
    ImGui::End();
    ImGui::PopFont();
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
