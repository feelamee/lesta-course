#include "tetramino.hpp"

#include "game_scene.hpp"

#include <nano/engine.hpp>
#include <nano/error.hpp>
#include <nano/texture2D.hpp>

#include <memory>
#include <random>

namespace tetris
{

tetramino::tetramino(type p_t, color c, nano::vec2f p_block_size)
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

    m_state = state::falling;
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
        break;

    case type::J:
        blocks_positions[0] = { 3, 21 }; //      3  4  5
        blocks_positions[1] = { 3, 20 }; // 21  [ ]
        blocks_positions[2] = { 4, 20 }; // 20  [ ][ ][ ]
        blocks_positions[3] = { 5, 20 }; //
        break;

    case type::L:
        blocks_positions[0] = { 3, 20 }; //      3  4  5
        blocks_positions[1] = { 4, 20 }; // 21        [ ]
        blocks_positions[2] = { 5, 20 }; // 20  [ ][ ][ ]
        blocks_positions[3] = { 5, 21 }; //
        break;

    case type::O:
        blocks_positions[0] = { 3, 21 }; //      3  4
        blocks_positions[1] = { 4, 21 }; // 21  [ ][ ]
        blocks_positions[2] = { 3, 20 }; // 20  [ ][ ]
        blocks_positions[3] = { 4, 20 }; //
        break;

    case type::S:
        blocks_positions[0] = { 3, 20 }; //     3  4  5
        blocks_positions[1] = { 4, 20 }; // 21    [ ][ ]
        blocks_positions[2] = { 4, 21 }; // 20 [ ][ ]
        blocks_positions[3] = { 5, 21 }; //
        break;

    case type::T:
        blocks_positions[0] = { 3, 20 }; //     3  4  5
        blocks_positions[1] = { 4, 20 }; // 21    [ ]
        blocks_positions[2] = { 4, 21 }; // 20 [ ][ ][ ]
        blocks_positions[3] = { 5, 20 }; //
        break;

    case type::Z:
        blocks_positions[0] = { 3, 21 }; //     3  4  5
        blocks_positions[1] = { 4, 21 }; // 21 [ ][ ]
        blocks_positions[2] = { 4, 20 }; // 20    [ ][ ]
        blocks_positions[3] = { 5, 20 }; //
        break;
    }
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
        return;

    for (auto& [_, j] : blocks_positions)
    {
        --j;
    }
}

void
tetramino::rotate90()
{
}

void
tetramino::rotate270()
{
}

void
tetramino::rshift()
{
    for (auto& [i, _] : positions())
    {
        ++i;
    }
}

void
tetramino::lshift()
{
    for (auto& [i, _] : positions())
    {
        --i;
    }
}

void
tetramino::lock()
{
    m_state = state::locked;
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
