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
    enum flag
    {
        video = 0x00000020,
        audio = 0x00000010,
        events = 0x00004000,
        all = video | audio | events
    };

    struct window_t
    {
        vec2f size{ 720, 960 };
    } window;

    int initialize(int init_flags);
    void finalize();

    void new_frame();
    void renderUI();
    int swap_buffers();

    static engine& instance();

    engine() = default;
    ~engine();

private:
    struct impl_t;
    std::shared_ptr<impl_t> impl;
    int flags;
};

} // namespace nano

#endif // ENGINE_HPP
