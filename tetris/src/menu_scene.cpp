#include "menu_scene.hpp"

#include "game_scene.hpp"
#include "tetramino.hpp"

#include <nano/error.hpp>
#include <nano/utils.hpp>

namespace tetris
{

menu_scene::menu_scene(float p_width)
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

    auto win = nano::engine::instance()->window;
    pixels_size = { p_width, p_width * game_scene::height / game_scene::width };
    block_size =
        pixels_size / nano::vec2f(game_scene::width, game_scene::height);
    pixels_size_visible = win.size;

    fill_states();
}

void
menu_scene::start()
{
    bg_music.play();
}

void
menu_scene::process(delta_t delta)
{
    process_internal(delta);

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
    ImGui::PushStyleColor(ImGuiCol_Button, nano::color::hex(50, 0, 200, 50));
    if (ImGui::Button("Start", { start_button_size, 120 }))
    {
        auto game = std::make_shared<game_scene>(pixels_size.x);
        e->scenarist.push(game);
    }

    const float settings_button_size = e->window.size.x / 4 * 3;
    // ImGui::CalcTextSize("Settings").x + 100;
    {
        ImGui::SetCursorPosX(0);

        const float off = (ImGui::GetWindowWidth() - settings_button_size) / 2;
        if (off > 0)
            ImGui::SetCursorPos({ off, ImGui::GetWindowHeight() / 8 * 3 });
    }

    if (ImGui::Button("Settings", { settings_button_size, 120 }))
    {
        // settings
    }
    ImGui::PopStyleColor();
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
    reset_animation();
    bg_music.pause();
}

void
menu_scene::stop()
{
    bg_music.stop();
}

void
menu_scene::draw(nano::drawable::state s) const
{
    auto&& win = nano::engine::instance()->window;
    nano::transform2D tr;
    tr.move(-pixels_size_visible / win.size / nano::vec2f{ 1, win.ratio });
    s.transform.combine(tr);

    if (nullptr != falling)
    {
        falling->draw(s);
    }
    using namespace std::placeholders;
    std::for_each(
        begin(blocks), end(blocks), std::bind(&tetramino::draw, _1, s));
}

//  methods for animation

void
menu_scene::reset_animation()
{
    state = 0;
    blocks.clear();
    falling = nullptr;
}
void
menu_scene::fill_states()
{
    states.reserve(30);

    auto spawn_block = [this](tetramino::type t)
    {
        if (nullptr == falling or falling->is_locked())
        {
            auto&& block = std::make_shared<tetramino>(t, block_size);
            add(block);
            ++state;
        }
    };

    using mov = std::pair<int, int>;
    using rot = std::pair<int, int>;
    using rots = std::vector<rot>;
    using movs = std::vector<mov>;

    auto transform =
        [this, i{ 0 }, j{ 0 }, k{ 0 }](movs _movs = {}, rots _rots = {}) mutable
    {
        if (j < _movs.size() and i == _movs[j].first)
        {
            xshift_falling(_movs[j].second);
            ++j;
        }
        if (k < _rots.size() and i == _rots[k].first)
        {
            rot_falling(_rots[k].second);
            ++k;
        }
        ++i;
        if (nullptr == falling or falling->is_locked())
        {
            ++state;
            i = 0;
            j = 0;
            k = 0;
        }
    };

    states.push_back(std::bind(spawn_block, tetramino::type::O));

    states.push_back(std::bind(spawn_block, tetramino::type::J));
    states.push_back(
        std::bind(transform, movs{ mov(3, 1) }, rots{ rot(7, 1) }));

    states.push_back(std::bind(spawn_block, tetramino::type::L));
    states.push_back(std::bind(transform, movs{}, rots{ rot(2, -1) }));

    states.push_back(std::bind(spawn_block, tetramino::type::L));
    states.push_back(
        std::bind(transform, movs{}, rots{ rot(2, -1), rot(8, -1) }));
    states.push_back(std::bind(spawn_block, tetramino::type::J));
    states.push_back(std::bind(transform,
                               movs{ mov(2, 1), mov(10, 1) },
                               rots{ rot(8, 1), rot(9, 1) }));

    states.push_back(
        [this, i = 0]() mutable
        {
            if (i > 10)
            {
                ++state;
            }
            ++i;
        });
    states.push_back(
        [this, i = 0]() mutable
        {
            if (i == 12)
            {
                ++state;
                return;
            }
            if (i % 2 == 0)
            {
                delete_row(0);
            }
            ++i;
        });

    states.push_back(std::bind(spawn_block, tetramino::type::S));
    states.push_back(
        std::bind(transform, movs{ mov(12, -1) }, rots{ rot(5, 1) }));

    states.push_back(std::bind(spawn_block, tetramino::type::Z));
    states.push_back(
        std::bind(transform, movs{ mov(7, 1), mov(8, 1) }, rots{ rot(5, -1) }));

    states.push_back(std::bind(spawn_block, tetramino::type::T));
    states.push_back(std::bind(transform,
                               movs{ mov(7, -1), mov(8, -1) },
                               rots{ rot(5, 1), rot(6, 1) }));

    states.push_back(std::bind(spawn_block, tetramino::type::S));
    states.push_back(std::bind(transform, movs{ mov(7, -1), mov(8, -1) }));

    states.push_back(std::bind(spawn_block, tetramino::type::L));
    states.push_back(std::bind(
        transform, movs{ mov(3, 1), mov(17, -1) }, rots{ rot(2, -1) }));

    states.push_back(std::bind(spawn_block, tetramino::type::T));
    states.push_back(std::bind(transform,
                               movs{ mov(2, 1), mov(3, 1), mov(12, 1) },
                               rots{ rot(5, 1), rot(6, 1) }));

    states.push_back(std::bind(spawn_block, tetramino::type::J));
    states.push_back(
        std::bind(transform, movs{ mov(10, 1) }, rots{ rot(3, 1) }));

    states.push_back(std::bind(spawn_block, tetramino::type::Z));
    states.push_back(
        std::bind(transform,
                  movs{ mov(2, 1), mov(3, 1), mov(5, 1), mov(12, -1) },
                  rots{}));
    states.push_back(
        [this, i = 0]() mutable
        {
            if (i > 100)
            {
                ++state;
            }
            ++i;
        });
}

void
menu_scene::process_internal(delta_t delta)
{
    delay += delta;

    if (delay < max_delay)
    {
        return;
    }

    delay -= max_delay;

    states[state]();

    if (nullptr == falling)
    {
        return;
    }

    falling->process(delta);

    if (is_collide(falling))
    {
        falling->yshift(1);
        lock_falling();
    }
    else
    {
        for (auto&& block : blocks)
        {
            if (is_collide(falling, block))
            {
                falling->yshift(1);
                lock_falling();
                break;
            }
        }
    }
}

void
menu_scene::add(std::shared_ptr<tetramino> block)
{
    falling = block;
    block->start();
}

void
menu_scene::xshift_falling(const int step) const
{
    if (not falling)
        return;

    falling->xshift(step);
    if (is_collide(falling))
    {
        falling->xshift(-step);
        return;
    }

    for (auto&& block : blocks)
    {
        if (is_collide(falling, block))
        {
            falling->xshift(-step);
            return;
        }
    }
}

void
menu_scene::rot_falling(int times) const
{
    if (not falling)
    {
        return;
    }

    falling->rot(times);
    if (is_collide(falling))
    {
        falling->rot(-times);
        return;
    }

    for (auto&& block : blocks)
    {
        if (is_collide(falling, block))
        {
            falling->rot(-times);
            return;
        }
    }
}

void
menu_scene::rot90_falling() const
{
    rot_falling(1);
}

void
menu_scene::rot270_falling() const
{
    rot_falling(-1);
}

void
menu_scene::delete_row(const int row)
{
    for (auto&& block : blocks)
    {
        const auto v_positions = block->positions();
        for (const auto pos : v_positions)
        {
            if (pos.y == row)
            {
                block->remove(pos);
            }
        }
    }
    shift_down_all_higher(row);
}

void
menu_scene::shift_down_all_higher(const int row)
{
    if (row < 0 or row > game_scene::height)
    {
        return;
    }

    for (auto&& block : blocks)
    {
        for (auto& [x, y] : block->positions())
        {
            if (y > row)
            {
                --y;
            }
        }
    }
}

void
menu_scene::lock_falling()
{
    falling->lock();
    blocks.push_back(falling);
    falling = nullptr;
}

} // namespace tetris
