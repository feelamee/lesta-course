#include "SDL_keycode.h"
#include <nano/imgui_backend.hpp>

#include <nano/engine.hpp>

#include <imgui.h>

#include <SDL3/SDL.h>

namespace imgui
{

struct backend_data
{
    SDL_Window* Window;
    SDL_Renderer* Renderer;
    Uint64 Time;
    Uint32 MouseWindowID;
    int MouseButtonsDown;
    SDL_Cursor* MouseCursors[ImGuiMouseCursor_COUNT];
    SDL_Cursor* LastMouseCursor;
    int PendingMouseLeaveFrame;
    char* ClipboardTextData;
    bool MouseCanUseGlobalState;

    backend_data()
    {
        memset((void*)this, 0, sizeof(*this));
    }
};

static backend_data*
get_backend_data()
{
    return ImGui::GetCurrentContext()
               ? (backend_data*)ImGui::GetIO().BackendPlatformUserData
               : nullptr;
}

static void
update_key_mods(nano::keymod key_mods)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddKeyEvent(ImGuiMod_Ctrl, (key_mods & nano::keymod::km_ctrl) != 0);
    io.AddKeyEvent(ImGuiMod_Shift, (key_mods & nano::keymod::km_shift) != 0);
    io.AddKeyEvent(ImGuiMod_Alt, (key_mods & nano::keymod::km_alt) != 0);
    io.AddKeyEvent(ImGuiMod_Super, (key_mods & nano::keymod::km_gui) != 0);
}

static ImGuiKey keycode2ImGuikey(nano::keycode_t keycode);

bool
process_event(const nano::event* event)
{
    ImGuiIO& io = ImGui::GetIO();
    backend_data* bd = get_backend_data();
    assert(nullptr != bd);

    auto win_size = nano::engine::instance()->window.size;
    using ev_t = nano::event::type_t;

    switch (event->type)
    {
    case ev_t::mouse_motion:
    {
        if (event->motion.mouseID == nano::event::mouse_button::touch_mouseID)
        {
            break;
        }
        io.AddMouseSourceEvent(ImGuiMouseSource_Mouse);
        io.AddMousePosEvent((float)event->motion.x, (float)event->motion.y);
        return true;
    }
    case ev_t::mouse_wheel:
    {
        if (event->motion.mouseID == nano::event::mouse_button::touch_mouseID)
        {
            break;
        }
        io.AddMouseSourceEvent(ImGuiMouseSource_Mouse);
        io.AddMouseWheelEvent(-event->wheel.x, event->wheel.y);
        return true;
    }
    case ev_t::mouse_button_down:
    case ev_t::mouse_button_up:
    {
        if (event->mouse.mouseID == nano::event::mouse_button::touch_mouseID)
        {
            break;
        }
        int mouse_button = -1;
        if (event->mouse.button == nano::event::mouse_button::left)
        {
            mouse_button = 0;
        }
        if (event->mouse.button == nano::event::mouse_button::right)
        {
            mouse_button = 1;
        }
        if (event->mouse.button == nano::event::mouse_button::middle)
        {
            mouse_button = 2;
        }
        if (mouse_button == -1)
        {
            break;
        }
        io.AddMouseSourceEvent(ImGuiMouseSource_Mouse);
        io.AddMousePosEvent(event->mouse.x, event->mouse.y);
        io.AddMouseButtonEvent(mouse_button,
                               event->type == ev_t::mouse_button_down);
        bd->MouseButtonsDown =
            (event->type == ev_t::mouse_button_down)
                ? (bd->MouseButtonsDown | (1 << mouse_button))
                : (bd->MouseButtonsDown & ~(1 << mouse_button));
        return true;
    }
    case ev_t::finger_down:
    case ev_t::finger_up:
    {
        io.AddMouseSourceEvent(ImGuiMouseSource_TouchScreen);
        io.AddMousePosEvent(event->touch.x * win_size.x,
                            event->touch.y * win_size.y);
        io.AddMouseButtonEvent(0, event->type == ev_t::finger_down);
        bd->MouseButtonsDown = (event->type == ev_t::finger_down)
                                   ? (bd->MouseButtonsDown | 1)
                                   : (bd->MouseButtonsDown & ~1);
        return true;
    }
    case ev_t::finger_motion:
    {
        io.AddMouseSourceEvent(ImGuiMouseSource_TouchScreen);
        io.AddMousePosEvent((event->touch.x + event->touch.dx) * win_size.y,
                            (event->touch.y + event->touch.dy) * win_size.y);
        return true;
    }
    case ev_t::text_input:
    {
        io.AddInputCharactersUTF8(event->input.text);
        return true;
    }
    case ev_t::key_down:
    case ev_t::key_up:
    {
        update_key_mods((nano::keymod)event->kb.key.mod);
        ImGuiKey key = keycode2ImGuikey(event->kb.key.keycode);
        io.AddKeyEvent(key, (event->type == ev_t::key_down));
        io.SetKeyEventNativeData(
            key, event->kb.key.keycode, event->kb.key.scancode);
        return true;
    }
    case ev_t::window_mouse_enter:
    {
        bd->MouseWindowID = event->win.windowID;
        bd->PendingMouseLeaveFrame = 0;
        return true;
    }
    case ev_t::window_mouse_leave:
    {
        bd->PendingMouseLeaveFrame = ImGui::GetFrameCount() + 1;
        return true;
    }
    case ev_t::window_focus_gain:
        io.AddFocusEvent(true);
        return true;
    case ev_t::window_focus_lost:
        io.AddFocusEvent(false);
        return true;
    }
    return false;
}

static ImGuiKey
keycode2ImGuikey(nano::keycode_t keycode)
{
    // clang-format off
    switch(static_cast<SDL_Keycode>(keycode))
    {
    case SDLK_TAB: return ImGuiKey_Tab;
    case SDLK_LEFT: return ImGuiKey_LeftArrow;
    case SDLK_RIGHT: return ImGuiKey_RightArrow;
    case SDLK_UP: return ImGuiKey_UpArrow;
    case SDLK_DOWN: return ImGuiKey_DownArrow;
    case SDLK_PAGEUP: return ImGuiKey_PageUp;
    case SDLK_PAGEDOWN: return ImGuiKey_PageDown;
    case SDLK_HOME: return ImGuiKey_Home;
    case SDLK_END: return ImGuiKey_End;
    case SDLK_INSERT: return ImGuiKey_Insert;
    case SDLK_DELETE: return ImGuiKey_Delete;
    case SDLK_BACKSPACE: return ImGuiKey_Backspace;
    case SDLK_SPACE: return ImGuiKey_Space;
    case SDLK_RETURN: return ImGuiKey_Enter;
    case SDLK_ESCAPE: return ImGuiKey_Escape;
    case SDLK_QUOTE: return ImGuiKey_Apostrophe;
    case SDLK_COMMA: return ImGuiKey_Comma;
    case SDLK_MINUS: return ImGuiKey_Minus;
    case SDLK_PERIOD: return ImGuiKey_Period;
    case SDLK_SLASH: return ImGuiKey_Slash;
    case SDLK_SEMICOLON: return ImGuiKey_Semicolon;
    case SDLK_EQUALS: return ImGuiKey_Equal;
    case SDLK_LEFTBRACKET: return ImGuiKey_LeftBracket;
    case SDLK_BACKSLASH: return ImGuiKey_Backslash;
    case SDLK_RIGHTBRACKET: return ImGuiKey_RightBracket;
    case SDLK_BACKQUOTE: return ImGuiKey_GraveAccent;
    case SDLK_CAPSLOCK: return ImGuiKey_CapsLock;
    case SDLK_SCROLLLOCK: return ImGuiKey_ScrollLock;
    case SDLK_NUMLOCKCLEAR: return ImGuiKey_NumLock;
    case SDLK_PRINTSCREEN: return ImGuiKey_PrintScreen;
    case SDLK_PAUSE: return ImGuiKey_Pause;
    case SDLK_KP_0: return ImGuiKey_Keypad0;
    case SDLK_KP_1: return ImGuiKey_Keypad1;
    case SDLK_KP_2: return ImGuiKey_Keypad2;
    case SDLK_KP_3: return ImGuiKey_Keypad3;
    case SDLK_KP_4: return ImGuiKey_Keypad4;
    case SDLK_KP_5: return ImGuiKey_Keypad5;
    case SDLK_KP_6: return ImGuiKey_Keypad6;
    case SDLK_KP_7: return ImGuiKey_Keypad7;
    case SDLK_KP_8: return ImGuiKey_Keypad8;
    case SDLK_KP_9: return ImGuiKey_Keypad9;
    case SDLK_KP_PERIOD: return ImGuiKey_KeypadDecimal;
    case SDLK_KP_DIVIDE: return ImGuiKey_KeypadDivide;
    case SDLK_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
    case SDLK_KP_MINUS: return ImGuiKey_KeypadSubtract;
    case SDLK_KP_PLUS: return ImGuiKey_KeypadAdd;
    case SDLK_KP_ENTER: return ImGuiKey_KeypadEnter;
    case SDLK_KP_EQUALS: return ImGuiKey_KeypadEqual;
    case SDLK_LCTRL: return ImGuiKey_LeftCtrl;
    case SDLK_LSHIFT: return ImGuiKey_LeftShift;
    case SDLK_LALT: return ImGuiKey_LeftAlt;
    case SDLK_LGUI: return ImGuiKey_LeftSuper;
    case SDLK_RCTRL: return ImGuiKey_RightCtrl;
    case SDLK_RSHIFT: return ImGuiKey_RightShift;
    case SDLK_RALT: return ImGuiKey_RightAlt;
    case SDLK_RGUI: return ImGuiKey_RightSuper;
    case SDLK_APPLICATION: return ImGuiKey_Menu;
    case SDLK_0: return ImGuiKey_0;
    case SDLK_1: return ImGuiKey_1;
    case SDLK_2: return ImGuiKey_2;
    case SDLK_3: return ImGuiKey_3;
    case SDLK_4: return ImGuiKey_4;
    case SDLK_5: return ImGuiKey_5;
    case SDLK_6: return ImGuiKey_6;
    case SDLK_7: return ImGuiKey_7;
    case SDLK_8: return ImGuiKey_8;
    case SDLK_9: return ImGuiKey_9;
    case SDLK_a: return ImGuiKey_A;
    case SDLK_b: return ImGuiKey_B;
    case SDLK_c: return ImGuiKey_C;
    case SDLK_d: return ImGuiKey_D;
    case SDLK_e: return ImGuiKey_E;
    case SDLK_f: return ImGuiKey_F;
    case SDLK_g: return ImGuiKey_G;
    case SDLK_h: return ImGuiKey_H;
    case SDLK_i: return ImGuiKey_I;
    case SDLK_j: return ImGuiKey_J;
    case SDLK_k: return ImGuiKey_K;
    case SDLK_l: return ImGuiKey_L;
    case SDLK_m: return ImGuiKey_M;
    case SDLK_n: return ImGuiKey_N;
    case SDLK_o: return ImGuiKey_O;
    case SDLK_p: return ImGuiKey_P;
    case SDLK_q: return ImGuiKey_Q;
    case SDLK_r: return ImGuiKey_R;
    case SDLK_s: return ImGuiKey_S;
    case SDLK_t: return ImGuiKey_T;
    case SDLK_u: return ImGuiKey_U;
    case SDLK_v: return ImGuiKey_V;
    case SDLK_w: return ImGuiKey_W;
    case SDLK_x: return ImGuiKey_X;
    case SDLK_y: return ImGuiKey_Y;
    case SDLK_z: return ImGuiKey_Z;
    case SDLK_F1: return ImGuiKey_F1;
    case SDLK_F2: return ImGuiKey_F2;
    case SDLK_F3: return ImGuiKey_F3;
    case SDLK_F4: return ImGuiKey_F4;
    case SDLK_F5: return ImGuiKey_F5;
    case SDLK_F6: return ImGuiKey_F6;
    case SDLK_F7: return ImGuiKey_F7;
    case SDLK_F8: return ImGuiKey_F8;
    case SDLK_F9: return ImGuiKey_F9;
    case SDLK_F10: return ImGuiKey_F10;
    case SDLK_F11: return ImGuiKey_F11;
    case SDLK_F12: return ImGuiKey_F12;
    }
    // clang-format on
    return ImGuiKey_None;
}
} // namespace imgui
