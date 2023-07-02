#include "game_scene.hpp"

#include "tetramino.hpp"

#include <compare>
#include <nano/engine.hpp>
#include <nano/error.hpp>

#include <imgui.h>

#include <algorithm>
#include <chrono>
#include <climits>
#include <cstdlib>
#include <functional>
#include <memory>
#include <sys/stat.h>
#include <vector>

namespace tetris
{

game_scene::game_scene(float p_width)
    : node()
{
    int err_code = bg_beat.load(nano::engine::instance()->assets_path() /
                                "8bit-music.wav");
    if (EXIT_SUCCESS != err_code)
    {
        LOG_DEBUG("Fail while loading background beat sound for game scene");
    }
    else
    {
        bg_beat.volume(30);
        bg_beat.loop = true;
    }

    err_code =
        death.load(nano::engine::instance()->assets_path() / "death.wav");
    if (EXIT_SUCCESS != err_code)
    {
        LOG_DEBUG("Fail while loading death sound for game scene");
    }
    else
    {
        death.volume(100);
    }

    err_code = collision.load(nano::engine::instance()->assets_path() /
                              "tetramino-collision.wav");
    if (EXIT_SUCCESS != err_code)
    {
        LOG_DEBUG(
            "Fail while loading tetramino collision sound for game scene");
    }
    else
    {
        collision.volume(30);
    }

    auto win_size = nano::engine::instance()->window.size;
    pixels_size = { p_width, p_width * height / width };
    block_size = pixels_size / nano::vec2f(width, height);
    pixels_size_visible = win_size;

    subscribe_on_events();
}

void
game_scene::subscribe_on_events() const
{
    auto&& e = nano::engine::instance();
    using ev_t = nano::event::type_t;
    nano::event ev;

    // DONT TOUCH, this is super painstaking fitted code to
    // process touch events fine
    auto mouse_motion_callback = [this, e](auto ev)
    {
        if (is_motion)
        {
            // xoffset += ev.motion.xrel;
            xoffset += ev.touch.dx * e->window.size.x;
        }
        // just need -1, 0, or 1 to move tetramino
        int div = (xoffset / block_size.x);
        div %= 2;
        xshift_falling(div);
        xoffset -= div * block_size.x;
        is_motion = true;
    };

    auto mouse_pressed_motion_callback =
        [this, mouse_motion_callback, e](auto ev)
    {
        // if (ev.motion.yrel > std::abs(ev.motion.xrel))
        if (ev.touch.dy > std::abs(ev.touch.dx))
        {
            // yoffset += ev.motion.yrel > 0 ? ev.motion.yrel : 0;
            yoffset += e->window.size.y * (ev.touch.dy > 0 ? ev.touch.dy : 0);
            if (yoffset > block_size.y)
            {
                shift_down();
                yoffset -= block_size.y;
                return;
            }
        }

        mouse_motion_callback(ev);
    };

    // ev.type = ev_t::mouse_motion;
    ev.type = ev_t::finger_motion;
    // ev.motion.state = nano::event::button_state::pressed;
    e->supplier.subscribe({ ev, id }, mouse_pressed_motion_callback);
    // ev.motion.state = nano::event::button_state::released;
    // e->supplier.subscribe({ ev, id }, mouse_motion_callback);

    // ev.type = ev_t::mouse_key_up;
    ev.type = ev_t::finger_up;
    // ev.mouse.button = nano::event::mouse_button::left;
    // ev.mouse.state = nano::event::button_state::released;
    e->supplier.subscribe({ ev, id },
                          [this](auto ev)
                          {
                              if (not is_motion)
                              {
                                  rot90_falling();
                              }
                              else
                              {
                                  is_motion = false;
                              }
                          });
    // std::bind(&game_scene::rot90_falling, this));

    ev.type = ev_t::quit;
    e->supplier.subscribe({ ev, id }, std::bind(&nano::engine::stop, e));

    ev.type = ev_t::window_close_request;
    e->supplier.subscribe({ ev, id }, std::bind(&nano::engine::stop, e));

    ev.type = ev_t::key_down;
    ev.kb.key.mod = 0;
    ev.kb.key.keycode = nano::keycode_t::kb_d;
    e->supplier.subscribe({ ev, id },
                          std::bind(&game_scene::xshift_falling, this, 1));

    ev.kb.key.keycode = nano::keycode_t::kb_a;
    e->supplier.subscribe({ ev, id },
                          std::bind(&game_scene::xshift_falling, this, -1));

    ev.kb.repeat = false;
    ev.kb.key.keycode = nano::keycode_t::kb_h;
    e->supplier.subscribe({ ev, id },
                          std::bind(&game_scene::rot270_falling, this));

    ev.kb.key.keycode = nano::keycode_t::kb_l;
    e->supplier.subscribe({ ev, id },
                          std::bind(&game_scene::rot90_falling, this));

    ev.kb.key.keycode = nano::keycode_t::kb_j;
    e->supplier.subscribe({ ev, id }, std::bind(&game_scene::shift_down, this));
}

void
game_scene::add(std::shared_ptr<tetramino> block)
{
    falling = block;
    block->start();
}

void
game_scene::start()
{
    add(std::make_shared<tetramino>(tetramino::random_type(), block_size));
    bg_beat.play();
}

void
game_scene::stop()
{
    bg_beat.stop();
}

void
game_scene::pause()
{
    bg_beat.pause();
}

void
game_scene::resume()
{
    bg_beat.play();
}

void
game_scene::xshift_falling(const int step) const
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
game_scene::shift_down() const
{
    if (not falling)
    {
        return;
    }

    delay += max_delay;
}

void
game_scene::rot90_falling() const
{
    if (not falling)
    {
        return;
    }

    falling->rot90();
    if (is_collide(falling))
    {
        falling->rot270();
        return;
    }

    for (auto&& block : blocks)
    {
        if (is_collide(falling, block))
        {
            falling->rot270();
            return;
        }
    }
}

void
game_scene::rot270_falling() const
{
    if (not falling)
    {
        return;
    }

    falling->rot270();
    if (is_collide(falling))
    {
        falling->rot90();
        return;
    }

    for (auto&& block : blocks)
    {
        if (is_collide(falling, block))
        {
            falling->rot90();
            return;
        }
    }
}

bool
game_scene::is_game_over() const
{
    for (auto&& block : blocks)
    {
        for (auto&& [_, y] : block->positions())
        {
            if (y > height_visible)
            {
                return true;
            }
        }
    }

    return false;
}

void
game_scene::delete_full_rows()
{
    for (int i{ 0 }; i < height_visible; ++i)
    {
        if (is_full_row(i))
        {
            delete_row(i);
            --i;
        }
    }
}

void
game_scene::delete_row(const int row)
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
    score += ariphmetic_progression_sum(1, 9, 1);
    using namespace std::chrono_literals;
    max_delay -= max_delay > 100ms ? 10ms : 0ms;
    shift_down_all_higher(row);
}

void
game_scene::shift_down_all_higher(const int row)
{
    if (row < 0 or row > height)
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

bool
game_scene::is_full_row(const int row) const
{
    if (row < 0 or row > height_visible)
    {
        return false;
    }

    int real_row_sum{ 0 };
    static int max_row_sum = ariphmetic_progression_sum(1, width - 1, 1);

    bool is_full_0Y{ false };
    for (auto&& block : blocks)
    {
        for (auto [x, y] : block->positions())
        {
            if (y == row)
            {
                if (0 == x)
                {
                    is_full_0Y = true;
                }
                else
                {
                    real_row_sum += x;
                }
            }
        }
    }

    return is_full_0Y and real_row_sum == max_row_sum;
}

int
game_scene::ariphmetic_progression_sum(const int begin,
                                       const int end,
                                       const int step)
{
    return (begin + end) / 2 * (end - begin + 1) / step;
}

void
game_scene::lock_falling()
{
    falling->lock();
    blocks.push_back(falling);
}

void
game_scene::draw_score() const
{
    auto&& e = nano::engine::instance();
    ImGui::SetNextWindowPos(
        { (e->window.size.x -
           ImGui::CalcTextSize(std::to_string(score).c_str()).x) /
              2,
          100 });
    ImGui::SetNextWindowSizeConstraints({ 250, 250 }, { 250, 250 });
    ImGui::Begin("Score",
                 nullptr,
                 ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration |
                     ImGuiWindowFlags_NoBackground);
    ImGui::PushStyleColor(ImGuiCol_Text,
                          nano::color::hex(nano::color{ 200, 100, 200 }));
    // ImGui::PushStyleVar(ImGuiStyleVar_a)
    ImGui::Text("%d", score);
    ImGui::PopStyleColor();
    ImGui::End();
}

void
game_scene::process(delta_t delta)
{
    delay += delta;

    draw_score();

    if (delay < max_delay)
    {
        return;
    }

    delay -= max_delay;

    falling->process(delta);
    ++score;

    bool is_collision{ false };
    if (is_collide(falling))
    {
        is_collision = true;
        falling->yshift(1);
        lock_falling();
        collision.play();
    }
    else
    {
        for (auto&& block : blocks)
        {
            if (is_collide(falling, block))
            {
                is_collision = true;
                falling->yshift(1);
                lock_falling();
                collision.play();
                break;
            }
        }
    }

    if (is_collision)
    {
        if (is_game_over())
        {
            bg_beat.stop();
            death.play_sync();
            nano::engine::instance()->scenarist.pop();
            return;
        }

        delete_full_rows();

        add(std::make_shared<tetramino>(tetramino::random_type(), block_size));
    }
}

void
game_scene::draw(nano::drawable::state s) const
{
    auto&& win = nano::engine::instance()->window;
    nano::transform2D tr;
    tr.move(-pixels_size_visible / win.size / nano::vec2f{ 1, win.ratio });
    s.transform.combine(tr);

    falling->draw(s);
    using namespace std::placeholders;
    std::for_each(
        begin(blocks), end(blocks), std::bind(&tetramino::draw, _1, s));
}

} // namespace tetris
