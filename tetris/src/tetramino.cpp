#include "tetramino.hpp"

#include <nano/engine.hpp>
#include <nano/texture2D.hpp>

#include <memory>

namespace tetris
{

tetramino::tetramino(type p_t, color c)
    : t(p_t)
{
    auto block_tex = std::make_shared<nano::texture2D>();
    auto assets_dir = nano::engine::instance()->assets_path();
    block_tex->load(assets_dir / "blue_block.png");
    block.texture(block_tex);
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
        blocks_positions[0] = { 3, 3 }; //
        blocks_positions[1] = { 3, 4 }; //     3  4  5  6
        blocks_positions[2] = { 3, 5 }; // 3  [ ][ ][ ][ ]
        blocks_positions[3] = { 3, 6 }; //
        break;

    case type::J:
        blocks_positions[0] = { 2, 3 }; //     3  5  5
        blocks_positions[1] = { 3, 3 }; // 2  [ ]
        blocks_positions[2] = { 3, 4 }; // 3  [ ][ ][ ]
        blocks_positions[3] = { 3, 5 }; //
        break;

    case type::L:
        blocks_positions[0] = { 3, 3 }; //     3  4  5
        blocks_positions[1] = { 3, 4 }; // 2        [ ]
        blocks_positions[2] = { 3, 5 }; // 3  [ ][ ][ ]
        blocks_positions[3] = { 2, 5 }; //
        break;

    case type::O:
        blocks_positions[0] = { 2, 3 }; //     3  4
        blocks_positions[1] = { 2, 4 }; // 2  [ ][ ]
        blocks_positions[2] = { 3, 3 }; // 3  [ ][ ]
        blocks_positions[3] = { 3, 4 }; //
        break;

    case type::S:
        blocks_positions[0] = { 2, 4 }; //    3  4  5
        blocks_positions[1] = { 2, 5 }; // 2    [ ][ ]
        blocks_positions[2] = { 3, 3 }; // 3 [ ][ ]
        blocks_positions[3] = { 3, 4 }; //
        break;

    case type::T:
        blocks_positions[0] = { 3, 3 }; //    3  4  5
        blocks_positions[1] = { 3, 4 }; // 2    [ ]
        blocks_positions[2] = { 3, 5 }; // 3 [ ][ ][ ]
        blocks_positions[3] = { 2, 4 }; //
        break;

    case type::Z:
        blocks_positions[0] = { 2, 3 }; //    3  4  5
        blocks_positions[1] = { 2, 4 }; // 2 [ ][ ]
        blocks_positions[2] = { 3, 4 }; // 3    [ ][ ]
        blocks_positions[3] = { 3, 5 }; //
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
    for (auto& [i, _] : blocks_positions)
    {
        ++i;
    }
}

void
tetramino::draw(nano::drawable::state s) const
{
    auto&& blocks_pos = positions();
    for (auto&& block_pos : blocks_pos)
    {
        block.move(block_pos);
        block.draw(s);
    }

    for (auto&& block_pos : blocks_pos)
    {
        block.move(-block_pos);
    }
}

} // namespace tetris
