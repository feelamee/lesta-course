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

    auto&& e = nano::engine::instance();
    nano::event ev;
    ev.type = nano::event::type_t::quit;
    e->supplier.subscribe({ ev, id }, [this]() { this->is_running = false; });

    ev.type = nano::event::type_t::window_close_request;
    e->supplier.subscribe({ ev, id }, [this]() { this->is_running = false; });
}

void
game_scene::add(std::shared_ptr<tetramino> block)
{
    blocks.push_back(block);
    block->start();
    for (const auto [i, j] : block->positions())
    {
        const int new_i = std::clamp(i, 0, width);
        const int new_j = std::clamp(j, 0, height);
        map[new_j][new_i] = block_state::falling;
    }
}

void
game_scene::start()
{
    add(std::make_shared<tetramino>(
        tetramino::random_type(), tetramino::color::blue, block_size));
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
game_scene::lock_falling()
{
    for (int i{ 0 }; i < height; ++i)
    {
        for (int j{ 0 }; j < width; ++j)
        {
            if (map[i][j] != block_state::falling)
                continue;

            map[i][j] = block_state::locked;
        }
    }
}

bool
game_scene::is_game_over() const
{

    for (int i{ 20 }; i < height; ++i)
    {
        for (int j{ 0 }; j < width; ++j)
        {
            if (map[i][j] == block_state::locked)
            {
                return true;
            }
        }
    }
    return false;
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
    bool is_collide{ false };
    for (int i{ 0 }; i < height; ++i)
    {
        for (int j{ 0 }; j < width; ++j)
        {
            if (map[i][j] != block_state::falling)
                continue;

            if (i == 0 or map[i - 1][j] == block_state::locked)
            {
                is_collide = true;
                blocks.back()->lock();
                goto loop_exit;
            }

            map[i][j] = block_state::empty;
            map[i - 1][j] = block_state::falling;
        }
    }
loop_exit:

    using std::placeholders::_1;
    std::for_each(
        begin(blocks), end(blocks), std::bind(&tetramino::process, _1, delta));

    if (is_collide)
    {
        lock_falling();
        if (is_game_over())
        {
            nano::engine::instance()->scenarist.pop();
            return;
        }

        add(std::make_shared<tetramino>(
            tetramino::random_type(), tetramino::color::blue, block_size));
    }
}

void
game_scene::draw(nano::drawable::state s) const
{
    auto&& win = nano::engine::instance()->window;
    nano::transform2D tr;
    tr.move(-pixels_size_visible / win.size / nano::vec2f{ 1, win.ratio });
    s.transform.combine(tr);

    using namespace std::placeholders;
    std::for_each(
        begin(blocks), end(blocks), std::bind(&tetramino::draw, _1, s));
}

} // namespace tetris
