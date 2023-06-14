#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <nano/event.hpp>
#include <nano/raii_wrapper.hpp>
#include <nano/texture.hpp>
#include <nano/transform.hpp>
#include <nano/vec.hpp>
#include <nano/vertbuf.hpp>

namespace nano
{

struct vertex;

class engine final
{
public:
    struct window_t
    {
        static constexpr vec2f size{ 720, 960 };
    } window;

    int initialize();
    void finalize();

    static void new_frame();
    static void renderUI();
    int swap_buffers();

    static engine& instance();

    engine() = default;
    ~engine();

private:
    struct impl_t;
    std::shared_ptr<impl_t> impl;
};

} // namespace nano

#endif // ENGINE_HPP
