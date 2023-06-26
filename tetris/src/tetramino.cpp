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
    {
        return;
    }

    for (auto& [_, y] : blocks_positions)
    {
        --y;
    }
}

bool
is_collideY(const std::shared_ptr<tetramino> lhs,
            const std::shared_ptr<tetramino> rhs)
{
    auto lpositions = lhs->positions();
    auto rpositions = rhs->positions();

    for (auto&& [lx, ly] : lpositions)
    {
        for (auto&& [rx, ry] : rpositions)
        {
            if (lx == rx and std::abs(ly - ry) == 1)
            {
                return true;
            }
        }
    }
    return false;
}

bool
is_collideX(const std::shared_ptr<tetramino> lhs,
            const std::shared_ptr<tetramino> rhs)
{
    auto lpositions = lhs->positions();
    auto rpositions = rhs->positions();

    for (auto&& [lx, ly] : lpositions)
    {
        for (auto&& [rx, ry] : rpositions)
        {
            if (ly == ry and std::abs(lx - rx) == 1)
            {
                return true;
            }
        }
    }
    return false;
}

bool
tetramino::is_on_floor() const
{
    for (auto [_, y] : positions())
    {
        if (y == 0)
        {
            return true;
        }
    }
    return false;
}

bool
tetramino::is_wall_left() const
{
    const auto [x, _] = min_positionX();

    if (x == 0)
    {
        return true;
    }
    return false;
}

bool
tetramino::is_wall_right() const
{
    const auto [x, _] = max_positionX();

    if (game_scene::width - x == 1)
    {
        return true;
    }
    return false;
}

nano::vec2i
tetramino::min_positionX() const
{
    const auto&& p_positions = positions();
    static auto less_x = [](auto&& lhs, auto&& rhs) { return lhs.x < rhs.x; };
    return *std::min_element(begin(p_positions), end(p_positions), less_x);
}

nano::vec2i
tetramino::min_positionY() const
{
    const auto&& p_positions = positions();
    static auto less_y = [](auto&& lhs, auto&& rhs) { return lhs.y < rhs.y; };
    return *std::min_element(begin(p_positions), end(p_positions), less_y);
}

nano::vec2i
tetramino::max_positionX() const
{
    const auto&& pos = positions();
    static auto less_x = [](auto&& lhs, auto&& rhs) { return lhs.x < rhs.x; };
    return *std::max_element(begin(pos), end(pos), less_x);
}

nano::vec2i
tetramino::max_positionY() const
{
    const auto&& pos = positions();
    static auto less_y = [](auto&& lhs, auto&& rhs) { return lhs.y < rhs.y; };
    return *std::max_element(begin(pos), end(pos), less_y);
}

nano::vec2f
tetramino::origin()
{
    float y{ 0 }, x{ 0 };
    auto [_1, min_y] = min_positionY();
    auto [_2, max_y] = max_positionY();
    auto [min_x, _3] = min_positionX();
    auto [max_x, _4] = max_positionX();

    for (int i{ min_x }; i <= max_x; ++i)
    {
        x += i;
    }
    for (int i{ min_y }; i <= max_y; ++i)
    {
        y += i;
    }

    LOG_DEBUG("origin minmax_x %d %d", min_x, max_x);
    LOG_DEBUG("origin minmax_y %d %d", min_y, max_y);
    return { x / (max_x - min_x + 1), y / (max_y - min_y + 1) };
}

void
tetramino::rot90()
{
    nano::transform2D rot90;
    LOG_DEBUG("orig %f %f", origin().x, origin().y);
    rot90.rotate(std::numbers::pi / 2, origin());
    nano::print(rot90);

    LOG_DEBUG("ROT90");
    for (auto& pos : blocks_positions)
    {
        LOG_DEBUG("old %d %d", pos.x, pos.y);
        nano::vec2f new_pos = rot90.combine(pos);
        LOG_DEBUG("float %f %f", new_pos.x, new_pos.y);
        pos = new_pos;
        LOG_DEBUG("new %d %d", pos.x, pos.y);
        LOG_DEBUG("");
    }

    LOG_DEBUG("");
    rotation_num++;
}

void
tetramino::rot270()
{
    nano::transform2D rot270;
    rot270.rotate(-std::numbers::pi / 2, origin());

    for (auto& pos : blocks_positions)
    {
        pos = rot270.combine(pos);
    }
}

void
tetramino::rshift()
{
    for (auto& [x, _] : blocks_positions)
    {
        ++x;
    }
}

void
tetramino::lshift()
{
    for (auto& [x, _] : blocks_positions)
    {
        --x;
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
