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
    int initialize() override;
    void finalize() override;

    int swap_buffers() override;

    engine()
        : iengine()
    {
    }

    ~engine();

public:
    raii_wrapper<int, void> sdl_init;
    std::shared_ptr<SDL_Window> window;
    std::shared_ptr<void> context;
};

} // namespace nano

#endif // ENGINE_HPP
