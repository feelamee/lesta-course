#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP

#include "tetramino.hpp"

#include <cstdint>
#include <nano/scene.hpp>

#include <chrono>
#include <memory>

namespace tetris
{

struct game_scene : public nano::node
{
    void add(std::shared_ptr<tetramino>);

    void start() override;
    void stop() override;

    void pause() override;
    void resume() override;

    void process(delta_t delta) override;
    void draw(nano::drawable::state) const override;

private:
    enum class block_state
    {
        empty,
        falling,
        locked,
    };

    std::list<std::shared_ptr<tetramino>> blocks;

    static constexpr int height = 24;
    static constexpr int width = 10;
    block_state map[height][width]{ block_state::empty };
    // 20 lines visible, and 4 hidden

    int score{ 0 };
    delta_t max_delay{ delta_t::period::den }; // depends on score
    delta_t delay{ 0 };
};

} // namespace tetris

#endif // GAME_SCENE_HPP
