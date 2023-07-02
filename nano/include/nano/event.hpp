#ifndef EVENT_HPP
#define EVENT_HPP

#include <chrono>
#include <cstdint>

namespace nano
{

struct event;

int poll_event(event* ev);

enum keycode_t : char
{
    kb_unknown = 0,

    kb_return = '\r',
    kb_escape = '\x1B',
    kb_backspace = '\b',
    kb_tab = '\t',
    kb_space = ' ',
    kb_exclaim = '!',
    kb_quotedbl = '"',
    kb_hash = '#',
    kb_percent = '%',
    kb_dollar = '$',
    kb_ampersand = '&',
    kb_quote = '\'',
    kb_leftparen = '(',
    kb_rightparen = ')',
    kb_asterisk = '*',
    kb_plus = '+',
    kb_comma = ',',
    kb_minus = '-',
    kb_period = '.',
    kb_slash = '/',
    kb_0 = '0',
    kb_1 = '1',
    kb_2 = '2',
    kb_3 = '3',
    kb_4 = '4',
    kb_5 = '5',
    kb_6 = '6',
    kb_7 = '7',
    kb_8 = '8',
    kb_9 = '9',
    kb_colon = ':',
    kb_semicolon = ';',
    kb_less = '<',
    kb_equals = '=',
    kb_greater = '>',
    kb_question = '?',
    kb_at = '@',

    /*
       Skip uppercase letters
     */

    kb_leftbracket = '[',
    kb_backslash = '\\',
    kb_rightbracket = ']',
    kb_caret = '^',
    kb_underscore = '_',
    kb_backquote = '`',
    kb_a = 'a',
    kb_b = 'b',
    kb_c = 'c',
    kb_d = 'd',
    kb_e = 'e',
    kb_f = 'f',
    kb_g = 'g',
    kb_h = 'h',
    kb_i = 'i',
    kb_j = 'j',
    kb_k = 'k',
    kb_l = 'l',
    kb_m = 'm',
    kb_n = 'n',
    kb_o = 'o',
    kb_p = 'p',
    kb_q = 'q',
    kb_r = 'r',
    kb_s = 's',
    kb_t = 't',
    kb_u = 'u',
    kb_v = 'v',
    kb_w = 'w',
    kb_x = 'x',
    kb_y = 'y',
    kb_z = 'z',
};

enum scancode_t
{
};

struct event
{
    using timestamp_t = std::chrono::duration<std::uint64_t, std::nano>;
    enum class type_t;

    struct keyboard
    {
        struct key_t
        {
            scancode_t scancode;
            keycode_t keycode;
            std::uint16_t mod;
        };

        type_t type;           /**< event_t::key_down or event_t::key_up */
        timestamp_t timestamp; /**< in nanoseconds */
        bool repeat;
        key_t key;
    };

    enum button_state : std::uint8_t
    {
        released = 0,
        pressed,
    };

    struct mouse_button
    {
        enum button_t : std::uint8_t
        {
            left = 1,
            middle,
            right,
        };
        static constexpr int touch_mouseID = -1;

        /**< event_t::mouse_button_down or event_t::mouse_button_up */
        type_t type;
        timestamp_t timestamp; /**< in nanoseconds */

        // int windowID;          /**< The window with mouse focus, if any */

        int mouseID; /**< The mouse instance id */
        button_t button;
        button_state state;
        std::uint8_t clicks; /**< single-click, double-click, etc. */
        float x;             /**< relative to window */
        float y;             /**< relative to window */
    };

    struct mouse_motion
    {
        type_t type;           /**< event_t::mouse_motion */
        timestamp_t timestamp; /**< In nanoseconds */
        int windowID;          /**< The window with mouse focus, if any */
        int mouseID;           /**< The mouse instance id */
        button_state state;    /**< The current button state */
        float x;               /**< X coordinate, relative to window */
        float y;               /**< Y coordinate, relative to window */
        float xrel;            /**< The relative motion in the X direction */
        float yrel;            /**< The relative motion in the Y direction */
    };

    struct mouse_wheel
    {
        enum direction_t
        {
            up,
            down,
            left,
            right,
        };

        type_t type;           /**< event_t::mouse_wheel */
        timestamp_t timestamp; /**< in nanoseconds */

        // int windowID;          /**< The window with mouse focus, if any */

        int mouseID; /**< The mouse instance id */
        float x;     /**< scroll, positive - right, negative - left */
        float y;     /**< scroll, positive from user, negative toward user */
        direction_t direction;
        float mouseX; /**< relative to window */
        float mouseY; /**< relative to window */
    };

    struct text_edit
    {
        type_t type;           /**< event::type::text_edit */
        timestamp_t timestamp; /**< in nanoseconds */
        int windowID;          /**< The window with keyboard focus, if any */
        char text[32];         /**< The editing text */
        std::int32_t start;    /**< The start cursor of selected editing text */
        std::int32_t length;   /**< The length of selected editing text */
    };

    struct text_input
    {
        type_t type;           /**< event::type::text_input */
        timestamp_t timestamp; /**< in nanoseconds */
        int windowID;          /**< The window with keyboard focus, if any */
        char text[32];         /**< The input text */
    };

    struct window
    {
        type_t type;           /**< event::type::window* */
        timestamp_t timestamp; /**< in nanoseconds */
        int windowID;
    };

    struct quit
    {
        type_t type; /**< event_t::quit */
        timestamp_t timestamp;
    };

    struct finger
    {
        type_t type;
        timestamp_t timestamp;
        int touchID; /**< The touch device id */
        int fingerID;
        float x;        /**< Normalized in the range 0...1 */
        float y;        /**< Normalized in the range 0...1 */
        float dx;       /**< Normalized in the range -1...1 */
        float dy;       /**< Normalized in the range -1...1 */
        float pressure; /**< Normalized in the range 0...1 */
        int windowID;   /**< The window underneath the finger, if any */
    };

    union
    {
        type_t type;
        keyboard kb;
        mouse_button mouse;
        mouse_motion motion;
        mouse_wheel wheel;
        finger touch;
        text_edit edit;
        text_input input;
        window win;
        quit exit;
    };
};

/**
 * @brief this events should match with SDL_EventType
 */
enum class event::type_t
{
    unknown = 0x000,

    quit = 0x100,

    key_down = 0x300,
    key_up,
    text_edit,
    text_input,
    keymap_change,

    finger_down = 0x700,
    finger_up,
    finger_motion,

    mouse_motion = 0x400,
    mouse_button_down,
    mouse_button_up,
    mouse_wheel,

    window_show = 0x202,      /**< Window has been shown */
    window_hide,              /**< Window has been hidden */
    window_expose,            /**< Window has been exposed and should be redrawn
                               */
    window_move,              /**< Window has been moved to data1, data2 */
    window_resize,            /**< Window has been resized to data1xdata2 */
    window_pixel_size_change, /**< The pixel size of the window has
                                            changed to data1xdata2 */
    window_minimize,          /**< Window has been minimized */
    window_maxmimize,         /**< Window has been maximized */
    window_restore,           /**< Window has been restored to normal size and
                                            position */
    window_mouse_enter,       /**< Window has gained mouse focus */
    window_mouse_leave,       /**< Window has lost mouse focus */
    window_focus_gain,        /**< Window has gained keyboard focus */
    window_focus_lost,        /**< Window has lost keyboard focus */
    window_close_request,     /**< The window manager requests that the */
};

enum keymod
{
    km_none = 0x0000,
    km_lshift = 0x0001,
    km_rshift = 0x0002,
    km_lctrl = 0x0040,
    km_rctrl = 0x0080,
    km_lalt = 0x0100,
    km_ralt = 0x0200,
    km_lgui = 0x0400,
    km_rgui = 0x0800,
    km_num = 0x1000,
    km_caps = 0x2000,
    km_mode = 0x4000,
    km_scroll = 0x8000,

    km_ctrl = km_lctrl | km_rctrl,
    km_shift = km_lshift | km_rshift,
    km_alt = km_lalt | km_ralt,
    km_gui = km_lgui | km_rgui
};

} // namespace nano

#endif // EVENT_HPP
