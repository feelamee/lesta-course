#include <color.hpp>
#include <image_loader.hpp>
#include <shader.hpp>
#include <triangle_render.hpp>

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

    constexpr size_t width = 375, height = 665;

    std::shared_ptr window(
        SDL_CreateWindow("Paint", width, height, SDL_WINDOW_OPENGL),
        SDL_DestroyWindow);
    ASSERT_SDL_ERROR(nullptr != window);

    std::shared_ptr sdl_renderer(
        SDL_CreateRenderer(window.get(), nullptr, SDL_RENDERER_ACCELERATED),
        SDL_DestroyRenderer);
    ASSERT_SDL_ERROR(nullptr != sdl_renderer);

    std::fstream src("./04-0-input-images/leo.ppm");
    canvas texture;
    ppm::load(src, texture);

    canvas img(width, height);
    triangle_render my_renderer(img);

    my_renderer.program = std::make_shared<blur>();
    auto brush = std::dynamic_pointer_cast<blur>(my_renderer.program);

    brush->uniform.buf = &texture;
    std::vector<vertex> rectangle;
    std::vector<size_t> indices;

    // clang-format off
    //                 x           y       r  g  b  tx ty
    rectangle = { { 0,         0,          0, 0, 0, 0, 0 },
                  { width - 1, 0,          1, 1, 1, 1, 0 },
                  { 0,         height - 1, 1, 1, 1, 0, 1 },
                  { width - 1, height - 1, 1, 1, 1, 1, 1 } };
    indices = { 0, 1, 3, 0, 2, 3 };
    // clang-format on

    std::shared_ptr sdl_surface(
        SDL_CreateSurfaceFrom(reinterpret_cast<void*>(&img(0, 0)),
                              width,
                              height,
                              width * sizeof(color),
                              SDL_PIXELFORMAT_RGB24),
        SDL_DestroySurface);
    ASSERT_SDL_ERROR(nullptr != sdl_surface);

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

            case SDL_EVENT_MOUSE_WHEEL:
                brush->uniform.radius += 2 * ev.wheel.y;
                break;

            case SDL_EVENT_KEY_DOWN:
                switch (ev.key.keysym.sym)
                {
                case SDLK_i: // increase
                    brush->uniform.strength += 1;
                    break;
                case SDLK_d: // decrease
                    if (brush->uniform.strength > 1)
                        brush->uniform.strength -= 1;
                    break;
                }
                break;
            }
        }

        my_renderer.rasterize(rectangle, indices);

        auto sdl_texture =
            SDL_CreateTextureFromSurface(sdl_renderer.get(), sdl_surface.get());
        ASSERT_SDL_ERROR(nullptr != sdl_texture);

        SDL_RenderClear(sdl_renderer.get());
        SDL_RenderTexture(sdl_renderer.get(), sdl_texture, nullptr, nullptr);
        SDL_RenderPresent(sdl_renderer.get());

        SDL_DestroyTexture(sdl_texture);
    }
    return EXIT_SUCCESS;
}
