#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <nano/event.hpp>
#include <nano/texture2D.hpp>
#include <nano/transform2D.hpp>
#include <nano/vec.hpp>
#include <nano/vertbuf.hpp>

#include <memory>

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

    void new_frame();
    void renderUI();
    int swap_buffers();

    static std::shared_ptr<engine> instance();

    ~engine();

    engine(const engine&) = delete;
    engine(engine&&) = delete;
    engine& operator=(const engine&) = delete;
    engine& operator=(engine&&) = delete;

private:
    engine() = default;

    static std::weak_ptr<engine> m_instance;

    struct impl_t;
    std::shared_ptr<impl_t> impl;
    int flags;
};

} // namespace nano

#endif // ENGINE_HPP
