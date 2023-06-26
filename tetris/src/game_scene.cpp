#include "game_scene.hpp"
#include "tetramino.hpp"

#include <nano/engine.hpp>
#include <nano/error.hpp>

#include <algorithm>
#include <functional>
#include <memory>

namespace tetris
{

game_scene::game_scene(float p_width, bool& p_is_running)
    : is_running(p_is_running)
    , node()
{
    pixels_size = { p_width, p_width * height / width };
    block_size = pixels_size / nano::vec2f{ width, height };
    pixels_size_visible = pixels_size;
    pixels_size_visible.y -= 4 * block_size.y;

    subscribe_on_events();
}

void
game_scene::subscribe_on_events() const
{
    auto&& e = nano::engine::instance();
    using ev_t = nano::event::type_t;
    nano::event ev;
    ev.type = ev_t::quit;
    e->supplier.subscribe({ ev, id }, [this]() { this->is_running = false; });

    ev.type = ev_t::window_close_request;
    e->supplier.subscribe({ ev, id }, [this]() { this->is_running = false; });

    ev.type = ev_t::key_down;
    ev.kb.key.keycode = nano::keycode_t::kb_d;
    e->supplier.subscribe({ ev, id }, [this]() { rshift_falling(); });

    ev.kb.key.keycode = nano::keycode_t::kb_a;
    e->supplier.subscribe({ ev, id }, [this]() { lshift_falling(); });

    ev.kb.key.keycode = nano::keycode_t::kb_h;
    e->supplier.subscribe({ ev, id }, [this]() { rot270_falling(); });

    ev.kb.key.keycode = nano::keycode_t::kb_l;
    e->supplier.subscribe({ ev, id }, [this]() { rot90_falling(); });
}

void
game_scene::add(std::shared_ptr<tetramino> block)
{
    if (falling)
    {
        blocks.push_back(falling);
    }

    falling = block;
    block->start();
}

void
game_scene::start()
{
    add(std::make_shared<tetramino>(tetramino::random_type(), block_size));
}

void
game_scene::stop()
{
}

void
game_scene::pause()
{
}

void
game_scene::resume()
{
}

void
game_scene::rshift_falling() const
{
    if (not falling)
        return;

    if (falling->is_wall_right())
    {
        return;
    }

    for (auto&& block : blocks)
    {
        if (is_collideX(falling, block))
        {
            return;
        }
    }
    falling->rshift();
}

void
game_scene::lshift_falling() const
{
    if (not falling)
        return;

    if (falling->is_wall_left())
    {
        return;
    }

    for (auto&& block : blocks)
    {
        if (is_collideX(falling, block))
        {
            return;
        }
    }
    falling->lshift();
}

void
game_scene::rot90_falling() const
{
    if (not falling)
    {
        return;
    }
    falling->rot90();
}

void
game_scene::rot270_falling() const
{
    if (not falling)
    {
        return;
    }
    falling->rot270();
}

bool
game_scene::is_game_over() const
{
    if (falling and falling->is_locked())
    {
        for (auto&& [_, y] : falling->positions())
        {
            if (y >= height_visible)
            {
                return true;
            }
        }
    }

    for (auto&& block : blocks)
    {
        for (auto&& [_, y] : block->positions())
        {
            if (y >= height_visible)
            {
                return true;
            }
        }
    }

    return false;
}

void
game_scene::lock_falling()
{
    falling->lock();
    blocks.push_back(falling);
}

// get rid of map and just use blocks to find collisions
// (if collisions founded, move tetramino one block higher and lock it)
void
game_scene::process(delta_t delta)
{
    delay += delta;

    if (delay < max_delay)
        return;

    delay -= max_delay;
    bool is_collision{ false };
    if (falling->is_on_floor())
    {
        is_collision = true;
        lock_falling();
    }
    else
    {
        for (auto&& block : blocks)
        {
            if (is_collideY(falling, block))
            {
                is_collision = true;
                lock_falling();
                break;
            }
        }
    }

    falling->process(delta);

    if (is_collision)
    {
        if (is_game_over())
        {
            nano::engine::instance()->scenarist.pop();
            return;
        }

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
