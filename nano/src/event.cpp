#include <nano/event.hpp>

#include <nano/error.hpp>

#include <SDL3/SDL_events.h>

#include <imgui_impl_sdl3.h>

#include <algorithm>
#include <chrono>
#include <ratio>

namespace nano
{

void
convert_sdl_event(SDL_Event* sdl_ev, event* ev)
{
    if (not sdl_ev or not ev)
        return;

    using ev_t = event::type_t;
    ev->type = static_cast<ev_t>(sdl_ev->type);
    ev->kb.timestamp = event::timestamp_t(sdl_ev->key.timestamp);
    switch (ev->type)
    {
    case ev_t::key_down:
    case ev_t::key_up:
        ev->kb.key.mod = sdl_ev->key.keysym.mod;
        ev->kb.key.scancode =
            static_cast<scancode_t>(sdl_ev->key.keysym.scancode);
        ev->kb.key.keycode = static_cast<keycode_t>(sdl_ev->key.keysym.sym);
        ev->kb.repeat = sdl_ev->key.repeat;
        break;

    case ev_t::mouse_key_down:
    case ev_t::mouse_key_up:
        ev->mouse.mouseID = sdl_ev->button.which;
        ev->mouse.button =
            static_cast<event::mouse_button::button_t>(sdl_ev->button.button);
        ev->mouse.clicks = sdl_ev->button.clicks;
        ev->mouse.state =
            static_cast<event::button_state>(sdl_ev->button.state);
        ev->mouse.x = sdl_ev->button.x;
        ev->mouse.y = sdl_ev->button.y;
        break;

    case ev_t::mouse_motion:
        ev->motion.state =
            static_cast<event::button_state>(sdl_ev->motion.state);
        ev->motion.mouseID = sdl_ev->motion.which;
        ev->motion.windowID = sdl_ev->motion.windowID;
        ev->motion.x = sdl_ev->motion.x;
        ev->motion.y = sdl_ev->motion.y;
        ev->motion.xrel = sdl_ev->motion.xrel;
        ev->motion.yrel = sdl_ev->motion.yrel;
        break;

    case ev_t::finger_motion:
        ev->touch.dx = sdl_ev->tfinger.dx;
        ev->touch.dy = sdl_ev->tfinger.dy;
    case ev_t::finger_down:
    case ev_t::finger_up:
        ev->touch.fingerID = sdl_ev->tfinger.fingerId;
        ev->touch.touchID = sdl_ev->tfinger.touchId;
        ev->touch.windowID = sdl_ev->tfinger.windowID;
        ev->touch.x = sdl_ev->tfinger.x;
        ev->touch.y = sdl_ev->tfinger.y;
        ev->touch.pressure = sdl_ev->tfinger.pressure;

    case ev_t::mouse_wheel:
        ev->wheel.mouseID = sdl_ev->wheel.which;
        ev->wheel.direction = static_cast<event::mouse_wheel::direction_t>(
            sdl_ev->wheel.direction);
        ev->wheel.x = sdl_ev->wheel.x;
        ev->wheel.y = sdl_ev->wheel.y;
        ev->wheel.mouseX = sdl_ev->wheel.mouseX;
        ev->wheel.mouseY = sdl_ev->wheel.mouseY;
        break;

    case ev_t::text_edit:
        std::copy(sdl_ev->edit.text,
                  sdl_ev->edit.text + SDL_TEXTEDITINGEVENT_TEXT_SIZE,
                  ev->edit.text);
        ev->edit.length = sdl_ev->edit.length;
        ev->edit.start = sdl_ev->edit.start;
        ev->edit.windowID = sdl_ev->edit.windowID;
        break;

    case ev_t::text_input:
        std::copy(sdl_ev->text.text,
                  sdl_ev->text.text + SDL_TEXTEDITINGEVENT_TEXT_SIZE,
                  ev->input.text);
        ev->input.windowID = sdl_ev->text.windowID;
        break;

    case ev_t::quit:
        break;

    case ev_t::window_close_request:
    case ev_t::window_expose:
    case ev_t::window_focus_gain:
    case ev_t::window_focus_lost:
    case ev_t::window_hide:
    case ev_t::window_show:
    case ev_t::window_maxmimize:
    case ev_t::window_minimize:
    case ev_t::window_mouse_enter:
    case ev_t::window_mouse_leave:
    case ev_t::window_move:
    case ev_t::window_resize:
    case ev_t::window_restore:
    case ev_t::window_pixel_size_change:
        ev->win.windowID = sdl_ev->window.windowID;
        break;

    default:
        ev->type = ev_t::unknown;
        break;
    }
}

int
poll_event(event* ev)
{
    if (nullptr == ev)
        return 0;

    static auto is_mouse_or_touch_ev = [](SDL_Event* ev)
    {
        return ev->type == SDL_EVENT_MOUSE_WHEEL or
               ev->type == SDL_EVENT_MOUSE_MOTION or
               ev->type == SDL_EVENT_MOUSE_BUTTON_DOWN or
               ev->type == SDL_EVENT_MOUSE_BUTTON_UP or
               ev->type == SDL_EVENT_FINGER_DOWN or
               ev->type == SDL_EVENT_FINGER_UP or
               ev->type == SDL_EVENT_FINGER_MOTION;
    };

    static auto is_keyboard_ev = [](SDL_Event* ev)
    {
        // clang-format off
        return ev->type == SDL_EVENT_KEY_DOWN or
               ev->type == SDL_EVENT_KEY_UP or
               ev->type == SDL_EVENT_TEXT_EDITING or
               ev->type == SDL_EVENT_TEXT_INPUT or
               ev->type == SDL_EVENT_KEYMAP_CHANGED or
               ev->type == SDL_EVENT_TEXT_EDITING_EXT;
        // clang-format on
    };

    static SDL_Event sdl_ev;

    bool imgui_capture{ true };
    while (imgui_capture)
    {
        int result = SDL_PollEvent(&sdl_ev);
        if (SDL_FALSE == result)
            return 0;

        if (ImGui::GetIO().WantCaptureMouse and is_mouse_or_touch_ev(&sdl_ev))
        {
            imgui_capture = true;
        }
        else if (ImGui::GetIO().WantCaptureKeyboard and is_keyboard_ev(&sdl_ev))
        {
            imgui_capture = true;
        }
        else
        {
            imgui_capture = false;
        }

        ImGui_ImplSDL3_ProcessEvent(&sdl_ev);
    }

    convert_sdl_event(&sdl_ev, ev);
    return 1;
}

} // namespace nano
