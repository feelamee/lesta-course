#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP

#include "tetramino.hpp"

#include <nano/scene.hpp>
#include <nano/sound.hpp>

#include <chrono>
#include <cstdint>
#include <memory>

namespace tetris
{

struct game_scene : public nano::node
{
    game_scene(float width, bool&);

    void add(std::shared_ptr<tetramino>);

    void start() override;
    void stop() override;

    void pause() override;
    void resume() override;

    void process(delta_t delta) override;
    void draw(nano::drawable::state) const override;

    static constexpr int height = 24;
    static constexpr int height_visible = 20;
    static constexpr int width = 10;
    nano::vec2f block_size;

private:
    static int ariphmetic_progression_sum(const int begin,
                                          const int end,
                                          const int step);
    void draw_score() const;
    void subscribe_on_events() const;
    void rshift_falling() const;
    void lshift_falling() const;
    void shift_down() const;
    void rot90_falling() const;
    void rot270_falling() const;
    bool is_game_over() const;
    void delete_full_rows();
    void shift_down_all_higher(const int row);
    bool is_full_row(const int row) const;
    void delete_row(const int row);
    void lock_falling();

    std::list<std::shared_ptr<tetramino>> blocks;
    std::shared_ptr<tetramino> falling;

    nano::vec2f pixels_size;
    nano::vec2f pixels_size_visible;

    int score{ 0 };
    delta_t max_delay{ delta_t::period::den }; // depends on score
    mutable delta_t delay{ 0 };
    nano::sound bg_beat;

    bool& is_running;
};

} // namespace tetris

#endif // GAME_SCENE_HPP
