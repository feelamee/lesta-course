#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <glad/glad.h>

#include "raii_wrapper.hpp"
#include <nano/event.hpp>
#include <nano/iengine.hpp>
#include <nano/texture.hpp>
#include <nano/transform.hpp>
#include <nano/vec.hpp>
#include <nano/vertbuf.hpp>

#include <vector>

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
    void set_uniform(const std::string&, const texture*) override;
    void set_uniform(const std::string&, const transform2D&) override;

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
};

} // namespace nano

#endif // ENGINE_HPP
