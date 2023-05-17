#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "raii_wrapper.hpp"
#include <event.hpp>
#include <glad/glad.h>
#include <iengine.hpp>
#include <vector>
#include <vertbuf.hpp>

struct SDL_Window;

namespace nano
{

struct vertex;

class engine final : public iengine
{
public:
    void render();
    int initialize(fs::path, fs::path) override;
    void finalize() override;
    void run() override;
    void set_program(GLuint);

    template <primitive_t primitive>
    static void render(const std::vector<vertex>&);

    int swap_buffers();

    engine()
        : iengine()
    {
    }

private:
    std::shared_ptr<SDL_Window> window;
    std::shared_ptr<void> context;
    raii_wrapper<int, void> sdl_init;
    GLuint program = 0;
};

} // namespace nano

#endif // ENGINE_HPP
