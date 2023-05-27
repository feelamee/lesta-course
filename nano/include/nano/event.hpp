#ifndef EVENT_HPP
#define EVENT_HPP

#include <chrono>
#include <cstdint>

namespace nano
{

enum keycode
{
    kb_UNKNOWN = 0,

    kb_RETURN = '\r',
    kb_ESCAPE = '\x1B',
    kb_BACKSPACE = '\b',
    kb_TAB = '\t',
    kb_SPACE = ' ',
    kb_EXCLAIM = '!',
    kb_QUOTEDBL = '"',
    kb_HASH = '#',
    kb_PERCENT = '%',
    kb_DOLLAR = '$',
    kb_AMPERSAND = '&',
    kb_QUOTE = '\'',
    kb_LEFTPAREN = '(',
    kb_RIGHTPAREN = ')',
    kb_ASTERISK = '*',
    kb_PLUS = '+',
    kb_COMMA = ',',
    kb_MINUS = '-',
    kb_PERIOD = '.',
    kb_SLASH = '/',
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
    kb_COLON = ':',
    kb_SEMICOLON = ';',
    kb_LESS = '<',
    kb_EQUALS = '=',
    kb_GREATER = '>',
    kb_QUESTION = '?',
    kb_AT = '@',

    /*
       Skip uppercase letters
     */

    kb_LEFTBRACKET = '[',
    kb_BACKSLASH = '\\',
    kb_RIGHTBRACKET = ']',
    kb_CARET = '^',
    kb_UNDERSCORE = '_',
    kb_BACKQUOTE = '`',
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

enum scancode
{
};

struct keysym
{
    scancode code;
    keycode sym;
    std::uint16_t mod;
};

using timestamp_t = std::chrono::duration<std::uint64_t, std::nano>;

enum class event_t
{
    unknown = 0,

    quit = 0x100,

    key_down = 0x300,
    key_up,
};

struct kb_event
{
    event_t type; /**< ::EVENT_KEY_DOWN or ::EVENT_KEY_UP */
    timestamp_t timestamp;
    bool repeat;
    keysym sym; /**< The key that was pressed or released */
};

struct quit_event
{
    event_t type; /**< EVENT_QUIT */
    timestamp_t timestamp;
};

union event
{
    event_t type;
    kb_event key;
    quit_event quit;
};

int poll_event(event* ev);

} // namespace nano

#endif // EVENT_HPP
