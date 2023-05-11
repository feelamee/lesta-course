#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "raii_wrapper.hpp"
#include <event.hpp>
#include <glad/glad.h>
#include <iengine.hpp>
#include <vector>

struct SDL_Window;
struct vertex;

enum class primitive_t
{
    points = GL_POINTS,
    lines = GL_LINES,
    line_loop = GL_LINE_LOOP,
    line_strip = GL_LINE_STRIP,
    triangles = GL_TRIANGLES,
    triangle_strip = GL_TRIANGLE_STRIP,
    triangle_fan = GL_TRIANGLE_FAN,
};

namespace nano
{

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
