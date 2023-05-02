#include <event.hpp>

namespace nano
{

int poll_event(event* ev)
{
    if (ev == nullptr)
        return 0;

    SDL_Event sdl_ev;
    int       result = SDL_PollEvent(&sdl_ev);

    switch (sdl_ev.type)
    {
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
            ev->type = sdl_ev.type == SDL_EVENT_KEY_DOWN ? EVENT_KEY_DOWN
                                                         : EVENT_KEY_UP;
            ev->key.timestamp = sdl_ev.key.timestamp;
            ev->key.keysym    = sdl_ev.key.keysym;
            ev->key.repeat    = sdl_ev.key.repeat;
            break;

        case SDL_EVENT_QUIT:
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            ev->type           = sdl_ev.type;
            ev->quit.timestamp = sdl_ev.quit.timestamp;
            break;

        default:
            result = 0;
    }

    return result;
}

} // namespace nano