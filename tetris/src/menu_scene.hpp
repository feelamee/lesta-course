#ifndef MENU_SCENE_HPP
#define MENU_SCENE_HPP

#include "tetramino.hpp"

#include <nano/engine.hpp>
#include <nano/scene.hpp>
#include <nano/sound.hpp>
#include <nano/texture2D.hpp>

#include <imgui.h>

#include <cstdlib>
#include <filesystem>
#include <memory>

namespace tetris
{

class menu_scene : public nano::node
{
public:
    menu_scene(float);
    void start() override;
    void process(delta_t) override;
    void resume() override;
    void pause() override;
    void stop() override;

    void draw(nano::drawable::state = {}) const override;

private:
    ImFont* font_bold{};
    ImFont* font_light{};
    nano::sound bg_music;
    nano::texture2D bg;

    std::vector<std::function<void()>> states;
    int state{ 0 };

    void fill_states();
    void reset_animation();

    void add(std::shared_ptr<tetramino>);
    void xshift_falling(const int step) const;
    void rot_falling(int times) const;
    void rot90_falling() const;
    void rot270_falling() const;
    void shift_down_all_higher(const int row);
    void delete_row(const int row);
    void lock_falling();
    void process_internal(delta_t);

    nano::vec2f pixels_size;
    nano::vec2f pixels_size_visible;
    nano::vec2f block_size;

    delta_t max_delay{ delta_t::period::den };
    mutable delta_t delay{ 0 };
    std::list<std::shared_ptr<tetramino>> blocks;
    std::shared_ptr<tetramino> falling;
};

} // namespace tetris

#endif // MENU_SCENE_HPP
