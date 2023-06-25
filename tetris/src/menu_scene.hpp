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

namespace tetris
{

class menu_scene : public nano::node
{
public:
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
};

} // namespace tetris

#endif // MENU_SCENE_HPP
