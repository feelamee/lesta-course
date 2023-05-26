#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <glad/glad.h>

#include "raii_wrapper.hpp"
#include <event.hpp>
#include <iengine.hpp>
#include <texture.hpp>
#include <transform.hpp>
#include <vec.hpp>
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
    int initialize() override;
    void finalize() override;
    void set_program(uint) override;
    void set_uniform(const std::string&, const texture*) override;
    void set_uniform(const std::string&, const transform2D*) override;

    int swap_buffers() override;

    engine()
        : iengine()
    {
    }

    ~engine();

private:
    raii_wrapper<int, void> sdl_init;
    std::shared_ptr<SDL_Window> window;
    std::shared_ptr<void> context;
    GLuint program = 0;
};

} // namespace nano

#endif // ENGINE_HPP
