#include "SDL_events.h"
#include "color.hpp"
#include "shader.hpp"
#include "triangle_render.hpp"

#include "image_loader.hpp"
#include <SDL3/SDL.h>
#include <fstream>
#include <functional>
#include <limits>
#include <memory>
#include <random>
#include <tuple>

template <typename constr_ret_t, typename destr_ret_t, typename... destr_args_t>
struct raii_wrapper
{
private:
    std::function<destr_ret_t(destr_args_t...)> m_destr;
    std::tuple<destr_args_t...> m_destr_args;

public:
    constr_ret_t constr_returned_v;
    raii_wrapper(constr_ret_t constr_ret_v,
                 std::function<destr_ret_t(destr_args_t...)> destr,
                 destr_args_t... argsDestr)
        : constr_returned_v(constr_ret_v)
    {
        m_destr = destr;
        m_destr_args = std::make_tuple<destr_args_t...>(argsDestr...);
    }
    ~raii_wrapper()
    {
        std::apply(m_destr, m_destr_args);
    }
};

#define ASSERT_SDL_ERROR(expr)                                                 \
    {                                                                          \
        if (!(expr))                                                           \
        {                                                                      \
            std::cerr << SDL_GetError() << std::endl;                          \
            return EXIT_FAILURE;                                               \
        }                                                                      \
    }

struct lupa : shader
{
    struct
    {
        float mouse_x;
        float mouse_y;
        float scale = 2;
        float radius = 30;
        canvas* buf = nullptr;
    } uniform;

    ublas::matrix<color, ublas::column_major> buf;

    vertex
    vertex_shader(const vertex& v) override
    {
        vertex result{ v };
        auto dx = v.x - uniform.mouse_x;
        auto dy = v.y - uniform.mouse_y;
        auto len = std::sqrt(dx * dx + dy * dy);
        if (len < uniform.radius)
        {
            result.x += 10 * dx / uniform.scale;
            result.y += 10 * dy / uniform.scale;
        }

        return result;
    }

    color
    fragment_shader(const vertex& v) override
    {
        auto width = (*uniform.buf).width() - 1;
        auto height = (*uniform.buf).height() - 1;
        float pos_x = width * v.tx;
        float pos_y = height * v.ty;

        float dx = v.x - uniform.mouse_x;
        float dy = v.y - uniform.mouse_y;
        float len = std::sqrt(dx * dx + dy * dy);
        if (std::abs(len - uniform.radius) < 0.5)
            return { 0, 0, 0 };

        color result = (*uniform.buf)(static_cast<position_t>(pos_x),
                                      static_cast<position_t>(pos_y));

        return result;
    }
};

namespace std
{
template <typename T>
shared_ptr(T* res, void (*deleter)(T*)) -> shared_ptr<T>;
} // namespace std

int
main()
{
    raii_wrapper<int, void> sdl_init(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS),
                                     SDL_Quit);
    ASSERT_SDL_ERROR(EXIT_SUCCESS == sdl_init.constr_returned_v);

    // constexpr size_t width = 747, height = 1328;
    constexpr size_t width = 370, height = 664;

    std::shared_ptr window(
        SDL_CreateWindow("Paint", width, height, SDL_WINDOW_OPENGL),
        SDL_DestroyWindow);
    ASSERT_SDL_ERROR(nullptr != window);

    std::shared_ptr sdl_renderer(
        SDL_CreateRenderer(window.get(), nullptr, SDL_RENDERER_ACCELERATED),
        SDL_DestroyRenderer);
    ASSERT_SDL_ERROR(nullptr != sdl_renderer);

    std::fstream src("./04-0-input-images/leo.ppm", std::ios::in);
    canvas texture;
    ppm::load(src, texture);

    canvas img(width, height);
    triangle_render my_renderer(img);
    my_renderer.program = std::make_shared<lupa>();

    auto brush = dynamic_cast<lupa*>(my_renderer.program.get());
    brush->uniform.buf = &texture;

    std::vector<vertex> rectangle = {
        { 0, 0, 1, 1, 1, 0, 0 },
        { width - 1, 0, 1, 1, 1, 1, 0 },
        { 0, height - 1, 1, 1, 1, 0, 1 },
        { width - 1, height - 1, 1, 1, 1, 1, 1 }
    };
    std::vector<size_t> indices = { 0, 1, 3, 0, 2, 3 };

    SDL_Event ev;
    bool is_running{ true };
    while (is_running)
    {
        while (SDL_PollEvent(&ev))
        {
            switch (ev.type)
            {
            case SDL_EVENT_QUIT:
                is_running = false;
                break;

            case SDL_EVENT_MOUSE_MOTION:
                brush->uniform.mouse_x = ev.motion.x;
                brush->uniform.mouse_y = ev.motion.y;
                break;
            }
        }

        my_renderer.rasterize(rectangle);
        std::shared_ptr surface(
            SDL_CreateSurfaceFrom(
                reinterpret_cast<void*>(&img.data().data()[0]),
                width,
                height,
                width * sizeof(color),
                SDL_PIXELFORMAT_RGB24),
            SDL_DestroySurface);
        ASSERT_SDL_ERROR(nullptr != surface);

        std::shared_ptr texture(
            SDL_CreateTextureFromSurface(sdl_renderer.get(), surface.get()),
            SDL_DestroyTexture);
        ASSERT_SDL_ERROR(nullptr != texture);

        SDL_RenderClear(sdl_renderer.get());
        SDL_RenderTexture(sdl_renderer.get(), texture.get(), nullptr, nullptr);
        SDL_RenderPresent(sdl_renderer.get());
    }
    return EXIT_SUCCESS;
}
