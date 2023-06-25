#include "game_scene.hpp"

#include <algorithm>

namespace tetris
{

void
game_scene::add(std::shared_ptr<tetramino> block)
{
    blocks.push_back(block);

    for (const auto [i, j] : block->positions())
    {
        const int new_i = std::clamp(i, 0, height);
        const int new_j = std::clamp(j, 0, width);
        map[new_i][new_j] = block_state::falling;
    }
}

void
game_scene::start()
{
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
game_scene::process(delta_t delta)
{
    for (int i{ height - 1 }; i >= 0; --i)
    {
        for (int j{ 0 }; j < width; ++j)
        {
        }
    }
}

void
game_scene::draw(nano::drawable::state s) const
{
}

} // namespace tetris
