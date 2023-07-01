#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <nano/event.hpp>
#include <nano/postman.hpp>
#include <nano/scene.hpp>
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
        vec2f size{ 500, 1000 };
        float ratio{ size.x / size.y };
    } window;

    scene_controller scenarist;
    postman supplier;

    int initialize(int init_flags);
    static std::filesystem::path assets_path();
    void new_frame();
    int render();
    void start();
    void stop();
    bool is_running();

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
    bool m_is_running{ false };
    int flags;
};

} // namespace nano

#endif // ENGINE_HPP
