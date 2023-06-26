#include "tetramino.hpp"

#include "game_scene.hpp"

#include <nano/engine.hpp>
#include <nano/error.hpp>
#include <nano/texture2D.hpp>
#include <nano/utils.hpp>

#include <climits>
#include <memory>
#include <random>

namespace tetris
{

tetramino::tetramino(type p_t, nano::vec2f p_block_size)
    : node()
    , t(p_t)
{
    auto block_tex = std::make_shared<nano::texture2D>();
    auto assets_dir = nano::engine::instance()->assets_path();

    std::filesystem::path filename{};

    switch (p_t)
    {
    case type::I:
        filename = "cyan-block.png";
        break;

    case type::J:
        filename = "blue-block.png";
        break;

    case type::L:
        filename = "orange-block.png";
        break;

    case type::O:
        filename = "yellow-block.png";
        break;

    case type::S:
        filename = "green-block.png";
        break;

    case type::T:
        filename = "purple-block.png";
        break;

    case type::Z:
        filename = "red-block.png";
        break;
    }

    block_tex->load(assets_dir / filename);
    block.texture(block_tex);
    block.size(p_block_size);
}

tetramino::type
tetramino::random_type()
{
    return static_cast<type>(rand() % 7);
}

std::vector<nano::vec2i>
tetramino::positions() const
{
    return blocks_positions;
}

std::vector<nano::vec2i>&
tetramino::positions()
{
    return blocks_positions;
}

void
tetramino::start()
{
    blocks_positions.resize(4);

    switch (t)
    {
    case type::I:
        blocks_positions[0] = { 3, 20 }; //
        blocks_positions[1] = { 4, 20 }; //      3  4  5  6
        blocks_positions[2] = { 5, 20 }; // 20  [ ][ ][ ][ ]
        blocks_positions[3] = { 6, 20 }; //
        m_origin = { 4.5, 20.5 };
        break;

    case type::J:
        blocks_positions[0] = { 3, 21 }; //      3  4  5
        blocks_positions[1] = { 3, 20 }; // 21  [ ]
        blocks_positions[2] = { 4, 20 }; // 20  [ ][ ][ ]
        blocks_positions[3] = { 5, 20 }; //
        m_origin = { 4, 20 };
        break;

    case type::L:
        blocks_positions[0] = { 3, 20 }; //      3  4  5
        blocks_positions[1] = { 4, 20 }; // 21        [ ]
        blocks_positions[2] = { 5, 20 }; // 20  [ ][ ][ ]
        blocks_positions[3] = { 5, 21 }; //
        m_origin = { 4, 20 };
        break;

    case type::O:
        blocks_positions[0] = { 3, 21 }; //      3  4
        blocks_positions[1] = { 4, 21 }; // 21  [ ][ ]
        blocks_positions[2] = { 3, 20 }; // 20  [ ][ ]
        blocks_positions[3] = { 4, 20 }; //
        m_origin = { 3.5, 20.5 };
        break;

    case type::S:
        blocks_positions[0] = { 3, 20 }; //     3  4  5
        blocks_positions[1] = { 4, 20 }; // 21    [ ][ ]
        blocks_positions[2] = { 4, 21 }; // 20 [ ][ ]
        blocks_positions[3] = { 5, 21 }; //
        m_origin = { 4, 20 };
        break;

    case type::T:
        blocks_positions[0] = { 3, 20 }; //     3  4  5
        blocks_positions[1] = { 4, 20 }; // 21    [ ]
        blocks_positions[2] = { 4, 21 }; // 20 [ ][ ][ ]
        blocks_positions[3] = { 5, 20 }; //
        m_origin = { 4, 21 };
        break;

    case type::Z:
        blocks_positions[0] = { 3, 21 }; //     3  4  5
        blocks_positions[1] = { 4, 21 }; // 21 [ ][ ]
        blocks_positions[2] = { 4, 20 }; // 20    [ ][ ]
        blocks_positions[3] = { 5, 20 }; //
        m_origin = { 4, 20 };
        break;
    }
    m_state = state::falling;
}

void
tetramino::stop()
{
}

void
tetramino::pause()
{
}

void
tetramino::resume()
{
}

void
tetramino::process(delta_t delta)
{
    if (m_state == state::locked)
    {
        return;
    }

    for (auto& [_, y] : blocks_positions)
    {
        --y;
    }
    --m_origin.y;
}

bool
is_collide(const std::shared_ptr<tetramino> lhs,
           const std::shared_ptr<tetramino> rhs)
{
    auto lpositions = lhs->positions();
    auto rpositions = rhs->positions();

    for (auto&& l : lpositions)
    {
        for (auto&& r : rpositions)
        {
            if (l == r)
            {
                return true;
            }
        }
    }

    return false;
}

bool
is_collide(const std::shared_ptr<tetramino> lhs)

{
    for (auto&& [x, y] : lhs->positions())
    {
        // clang-format off
        if (x < 0 or
            x >= game_scene::width or
            y < 0 or
            y >= game_scene::height)
        {
            return true;
        }
        // clang-format on
    }

    return false;
}

nano::vec2f
tetramino::origin()
{
    return m_origin;
}

void
tetramino::rot90()
{
    nano::transform2D rot90;
    rot90.rotate(3.1415 / 2, origin());

    for (auto& pos : blocks_positions)
    {
        nano::vec2f new_pos = rot90.combine(pos);
        pos = new_pos;
    }
}

void
tetramino::rot270()
{
    nano::transform2D rot270;
    rot270.rotate(-3.1415 / 2, origin());

    for (auto& pos : blocks_positions)
    {
        pos = rot270.combine(pos);
    }
}

void
tetramino::xshift(const int step)
{
    for (auto& [x, _] : blocks_positions)
    {
        x += step;
    }
    m_origin.x += step;
}

void
tetramino::yshift(const int step)
{
    for (auto& [_, y] : blocks_positions)
    {
        y += step;
    }
    m_origin.y += step;
}

void
tetramino::remove(const nano::vec2i pos)
{
    auto pos_it =
        std::find(begin(blocks_positions), end(blocks_positions), pos);
    if (pos_it != blocks_positions.end())
    {
        blocks_positions.erase(pos_it);
    }
}

void
tetramino::lock()
{
    m_state = state::locked;
}

bool
tetramino::is_locked() const
{
    return m_state == state::locked;
}

bool
tetramino::is_falling() const
{
    return m_state == state::falling;
}

void
tetramino::draw(nano::drawable::state s) const
{
    for (auto pos : positions())
    {
        block.position(pos * block.size());
        block.draw(s);
    }
}

} // namespace tetris
