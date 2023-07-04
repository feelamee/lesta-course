#include <cstdint>
#include <nano/postman.hpp>

#include <nano/engine.hpp>
#include <nano/error.hpp>
#include <nano/event.hpp>

#include <cstdlib>

namespace nano
{

bool
postman::equal_key_t::operator()(const key_t& fst, const key_t& snd) const
{
    if (fst.ev.type != snd.ev.type or fst.scene_id != snd.scene_id)
    {
        return false;
    }

    using ev_t = nano::event::type_t;

    switch (fst.ev.type)
    {
    case ev_t::key_down:
    case ev_t::key_up:
        return (fst.ev.kb.key.keycode == snd.ev.kb.key.keycode) and
               (fst.ev.kb.key.mod == snd.ev.kb.key.mod);

    case ev_t::mouse_motion:
        return (fst.ev.motion.state == snd.ev.motion.state);

    case ev_t::mouse_button_down:
    case ev_t::mouse_button_up:
        return (fst.ev.mouse.state == snd.ev.mouse.state) and
               (fst.ev.mouse.button == snd.ev.mouse.button);

    case ev_t::finger_down:
    case ev_t::finger_up:
    case ev_t::finger_motion:
        return true;

    case ev_t::quit:
    case ev_t::window_close_request:
        return true;

    default:
        return false;
    }
}

int
postman::hash_key_t::operator()(const key_t& key) const
{
    using ev_t = nano::event::type_t;

    switch (key.ev.type)
    {
    case ev_t::key_up:
    case ev_t::key_down:
        return std::hash<int>()(static_cast<int>(key.ev.type)) ^
               std::hash<int>()(key.ev.kb.key.keycode) ^
               std::hash<std::uint16_t>()(key.ev.kb.key.mod) ^
               std::hash<int>()(key.scene_id);

    case ev_t::mouse_motion:
        return std::hash<int>()(static_cast<int>(key.ev.type)) ^
               std::hash<event::button_state>()(key.ev.motion.state) ^
               std::hash<int>()(key.scene_id);

    case ev_t::mouse_button_down:
    case ev_t::mouse_button_up:
        return std::hash<int>()(static_cast<int>(key.ev.type)) ^
               std::hash<event::mouse_button::button_t>()(key.ev.mouse.button) ^
               std::hash<event::button_state>()(key.ev.mouse.state) ^
               std::hash<int>()(key.scene_id);

    case ev_t::finger_down:
    case ev_t::finger_up:
    case ev_t::finger_motion:
        return std::hash<int>()(static_cast<int>(key.ev.type)) ^
               std::hash<int>()(key.scene_id);

    case ev_t::quit:
    case ev_t::window_close_request:
        return std::hash<int>()(static_cast<int>(key.ev.type)) ^
               std::hash<int>()(key.scene_id);

    default:
        return std::hash<int>()(static_cast<int>(key.ev.type)) ^
               std::hash<int>()(key.scene_id); // ???
    }
}

int
postman::deliver(nano::event event)
{
    auto&& scenarist = engine::instance()->scenarist;
    if (not scenarist.top())
    {
        return EXIT_FAILURE;
    }

    key_t key{ event, scenarist.top()->id };
    if (recipients.contains(key))
    {
        recipients[key](event);
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}

int
postman::unsubscribe(key_t key)
{
    auto event_it = recipients.find(key);

    if (event_it != recipients.end())
    {
        recipients.erase(event_it);
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}

int
postman::subscribe(key_t key, eventHandler handler)
{
    if (recipients.contains(key))
    {
        return EXIT_FAILURE;
    }

    recipients[key] = handler;
    return EXIT_SUCCESS;
}

} // namespace nano
