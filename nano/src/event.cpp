#include <chrono>
#include <nano/event.hpp>

#include <SDL3/SDL_events.h>
#include <ratio>

namespace nano
{

void
convert_sdl_event(SDL_Event* sdl_ev, event* ev)
{
    switch (sdl_ev->type)
    {
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
        ev->type = static_cast<event_t>(sdl_ev->type);
        ev->key.timestamp = timestamp_t(sdl_ev->key.timestamp);
        ev->key.sym.mod = sdl_ev->key.keysym.mod;
        ev->key.sym.code = static_cast<scancode>(sdl_ev->key.keysym.scancode);
        ev->key.sym.sym = static_cast<keycode>(sdl_ev->key.keysym.sym);
        ev->key.repeat = sdl_ev->key.repeat;
        break;

    case SDL_EVENT_QUIT:
        ev->type = static_cast<event_t>(sdl_ev->type);
        ev->quit.timestamp = timestamp_t(sdl_ev->key.timestamp);
        break;

    default:
        ev->type = event_t::unknown;
    }
}

int
poll_event(event* ev)
{
    if (ev == nullptr)
        return 0;

    SDL_Event sdl_ev;
    int result = SDL_PollEvent(&sdl_ev);
    if (!result)
        return 0;

    convert_sdl_event(&sdl_ev, ev);
    return 1;
}

} // namespace nano
