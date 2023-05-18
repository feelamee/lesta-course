#include "texture.hpp"
#include <event.hpp>
#include <iengine.hpp>
#include <image_loader.hpp>
#include <shader.hpp>
#include <shape.hpp>
#include <texture.hpp>
#include <vertbuf.hpp>
#include <vertex.hpp>

#include <assert.h>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

int
main()
{
    nano::iengine& eng = nano::engine_instance();
    eng.initialize();

    // clang-format off
    nano::vertbuf buf(
        nano::primitive_t::triangle_strip,
        { { 1,  -1,    0, 0, 0,    0, 1},
          { 1,  1,    0, 0, 0,    0, 0},
          { 1,  0,    0, 0, 0,    1, 1},
          { 0,  1,    0, 0, 0,    1, 0},
          });
    // clang-format on

    std::ifstream FILE("/home/bella/code/lesta-course/engine/test/leo.ppm");
    nano::canvas img;
    nano::ppm::load(FILE, img);
    nano::texture tex(img);
    nano::shape tmp(buf, tex);

    // auto program = nano::setup_shaders("../engine/test/vertex-shader.vert",
    //                                    "../engine/test/fragment-shader.frag");

    auto program = nano::setup_shaders(
        "/home/bella/code/lesta-course/engine/test/vertex-shader.vert",
        "/home/bella/code/lesta-course/engine/test/fragment-shader.frag");
    if (-1 == program)
    {
        eng.finalize();
        return EXIT_FAILURE;
    }
    eng.set_program(program);

    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;

    bool is_running = true;
    while (is_running)
    {
        nano::event ev;
        while (poll_event(&ev))
        {
            std::cout << "[" << duration_cast<milliseconds>(ev.key.timestamp)
                      << "]";
            switch (ev.type)
            {
            case nano::EVENT_KEY_DOWN:
                std::cout << " EVENT_KEY_DOWN: "
                          << SDL_GetKeyName(ev.key.keysym.sym) << std::endl;
                break;

            case nano::EVENT_KEY_UP:
                std::cout << " EVENT_KEY_UP: "
                          << SDL_GetKeyName(ev.key.keysym.sym) << std::endl;

                if (ev.key.keysym.sym == SDLK_q)
                    is_running = false;
                break;
            }
        }

        nano::render(tmp);
        eng.swap_buffers();
    }

    eng.finalize();
    return EXIT_SUCCESS;
}
