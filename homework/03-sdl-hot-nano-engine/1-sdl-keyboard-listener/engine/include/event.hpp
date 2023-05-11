#ifndef EVENT_HPP
#define EVENT_HPP

#include <SDL3/SDL_events.h>
#include <cstdint>

namespace nano
{

enum
{
    EVENT_KEY_DOWN = 0x300,
    EVENT_KEY_UP,

    EVENT_QUIT = 0x100,
} event_t;

struct kb_event
{
    std::uint32_t type;      /**< ::EVENT_KEY_DOWN or ::EVENT_KEY_UP */
    std::uint64_t timestamp; /**< In nanoseconds*/
    bool repeat;
    SDL_Keysym keysym; /**< The key that was pressed or released */
};

struct quit_event
{
    std::uint32_t type;      /**< EVENT_QUIT */
    std::uint64_t timestamp; /**< In nanoseconds*/
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
