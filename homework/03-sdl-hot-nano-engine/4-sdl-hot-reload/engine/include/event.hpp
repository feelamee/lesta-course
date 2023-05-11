#ifndef EVENT_HPP
#define EVENT_HPP

#include <SDL3/SDL_events.h>

#include <chrono>
#include <cstdint>
using timestamp_t = std::chrono::duration<std::uint64_t, std::nano>;

namespace nano
{

enum
{
    EVENT_KEY_DOWN,
    EVENT_KEY_UP,

    EVENT_QUIT,
    EVENT_WINDOW_CLOSE_REQUESTED,
} event_t;

struct kb_event
{
    std::uint32_t type; /**< ::EVENT_KEY_DOWN or ::EVENT_KEY_UP */
    timestamp_t timestamp;
    bool repeat;
    SDL_Keysym keysym; /**< The key that was pressed or released */
};

struct quit_event
{
    std::uint32_t type; /**< EVENT_QUIT */
    timestamp_t timestamp;
};

union event
{
    std::uint32_t type;
    kb_event key;
    quit_event quit;
};

int
poll_event(event* ev);

} // namespace nano

#endif // EVENT_HPP
