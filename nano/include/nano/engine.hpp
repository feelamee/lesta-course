#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <nano/event.hpp>
#include <nano/raii_wrapper.hpp>
#include <nano/texture.hpp>
#include <nano/transform.hpp>
#include <nano/vec.hpp>
#include <nano/vertbuf.hpp>

#include <memory>
#include <vector>

namespace nano
{

struct vertex;

class engine final
{
public:
    int initialize();
    void finalize();

    static void new_frame();
    static void renderUI();
    int swap_buffers();

    void* window();
    void* context();

    engine() = default;
    ~engine();

private:
    struct impl_t;
    std::shared_ptr<impl_t> impl;
};

engine& engine_instance();

} // namespace nano

#endif // ENGINE_HPP
