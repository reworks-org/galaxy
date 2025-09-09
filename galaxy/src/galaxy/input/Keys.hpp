///
/// Keys.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_KEYS_HPP_
#define GALAXY_INPUT_KEYS_HPP_

#include <SDL3/SDL_keycode.h>

namespace galaxy
{
	///
	/// Enum class representing keys.
	///
	enum class Keys : SDL_Keycode
	{
		KEY_UNKNOWN              = SDLK_UNKNOWN,              /**< 0 */
		KEY_RETURN               = SDLK_RETURN,               /**< '\r' */
		KEY_ESCAPE               = SDLK_ESCAPE,               /**< '\x1B' */
		KEY_BACKSPACE            = SDLK_BACKSPACE,            /**< '\b' */
		KEY_TAB                  = SDLK_TAB,                  /**< '\t' */
		KEY_SPACE                = SDLK_SPACE,                /**< ' ' */
		KEY_EXCLAIM              = SDLK_EXCLAIM,              /**< '!' */
		KEY_DBLAPOSTROPHE        = SDLK_DBLAPOSTROPHE,        /**< '"' */
		KEY_HASH                 = SDLK_HASH,                 /**< '#' */
		KEY_DOLLAR               = SDLK_DOLLAR,               /**< '$' */
		KEY_PERCENT              = SDLK_PERCENT,              /**< '%' */
		KEY_AMPERSAND            = SDLK_AMPERSAND,            /**< '&' */
		KEY_APOSTROPHE           = SDLK_APOSTROPHE,           /**< '\'' */
		KEY_LEFTPAREN            = SDLK_LEFTPAREN,            /**< '(' */
		KEY_RIGHTPAREN           = SDLK_RIGHTPAREN,           /**< ')' */
		KEY_ASTERISK             = SDLK_ASTERISK,             /**< '*' */
		KEY_PLUS                 = SDLK_PLUS,                 /**< '+' */
		KEY_COMMA                = SDLK_COMMA,                /**< ',' */
		KEY_MINUS                = SDLK_MINUS,                /**< '-' */
		KEY_PERIOD               = SDLK_PERIOD,               /**< '.' */
		KEY_SLASH                = SDLK_SLASH,                /**< '/' */
		KEY_0                    = SDLK_0,                    /**< '0' */
		KEY_1                    = SDLK_1,                    /**< '1' */
		KEY_2                    = SDLK_2,                    /**< '2' */
		KEY_3                    = SDLK_3,                    /**< '3' */
		KEY_4                    = SDLK_4,                    /**< '4' */
		KEY_5                    = SDLK_5,                    /**< '5' */
		KEY_6                    = SDLK_6,                    /**< '6' */
		KEY_7                    = SDLK_7,                    /**< '7' */
		KEY_8                    = SDLK_8,                    /**< '8' */
		KEY_9                    = SDLK_9,                    /**< '9' */
		KEY_COLON                = SDLK_COLON,                /**< ':' */
		KEY_SEMICOLON            = SDLK_SEMICOLON,            /**< ';' */
		KEY_LESS                 = SDLK_LESS,                 /**< '<' */
		KEY_EQUALS               = SDLK_EQUALS,               /**< '=' */
		KEY_GREATER              = SDLK_GREATER,              /**< '>' */
		KEY_QUESTION             = SDLK_QUESTION,             /**< '?' */
		KEY_AT                   = SDLK_AT,                   /**< '@' */
		KEY_LEFTBRACKET          = SDLK_LEFTBRACKET,          /**< '[' */
		KEY_BACKSLASH            = SDLK_BACKSLASH,            /**< '\\' */
		KEY_RIGHTBRACKET         = SDLK_RIGHTBRACKET,         /**< ']' */
		KEY_CARET                = SDLK_CARET,                /**< '^' */
		KEY_UNDERSCORE           = SDLK_UNDERSCORE,           /**< '_' */
		KEY_GRAVE                = SDLK_GRAVE,                /**< '`' */
		KEY_A                    = SDLK_A,                    /**< 'a' */
		KEY_B                    = SDLK_B,                    /**< 'b' */
		KEY_C                    = SDLK_C,                    /**< 'c' */
		KEY_D                    = SDLK_D,                    /**< 'd' */
		KEY_E                    = SDLK_E,                    /**< 'e' */
		KEY_F                    = SDLK_F,                    /**< 'f' */
		KEY_G                    = SDLK_G,                    /**< 'g' */
		KEY_H                    = SDLK_H,                    /**< 'h' */
		KEY_I                    = SDLK_I,                    /**< 'i' */
		KEY_J                    = SDLK_J,                    /**< 'j' */
		KEY_K                    = SDLK_K,                    /**< 'k' */
		KEY_L                    = SDLK_L,                    /**< 'l' */
		KEY_M                    = SDLK_M,                    /**< 'm' */
		KEY_N                    = SDLK_N,                    /**< 'n' */
		KEY_O                    = SDLK_O,                    /**< 'o' */
		KEY_P                    = SDLK_P,                    /**< 'p' */
		KEY_Q                    = SDLK_Q,                    /**< 'q' */
		KEY_R                    = SDLK_R,                    /**< 'r' */
		KEY_S                    = SDLK_S,                    /**< 's' */
		KEY_T                    = SDLK_T,                    /**< 't' */
		KEY_U                    = SDLK_U,                    /**< 'u' */
		KEY_V                    = SDLK_V,                    /**< 'v' */
		KEY_W                    = SDLK_W,                    /**< 'w' */
		KEY_X                    = SDLK_X,                    /**< 'x' */
		KEY_Y                    = SDLK_Y,                    /**< 'y' */
		KEY_Z                    = SDLK_Z,                    /**< 'z' */
		KEY_LEFTBRACE            = SDLK_LEFTBRACE,            /**< '{' */
		KEY_PIPE                 = SDLK_PIPE,                 /**< '|' */
		KEY_RIGHTBRACE           = SDLK_RIGHTBRACE,           /**< '}' */
		KEY_TILDE                = SDLK_TILDE,                /**< '~' */
		KEY_DELETE               = SDLK_DELETE,               /**< '\x7F' */
		KEY_PLUSMINUS            = SDLK_PLUSMINUS,            /**< '\xB1' */
		KEY_CAPSLOCK             = SDLK_CAPSLOCK,             /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CAPSLOCK) */
		KEY_F1                   = SDLK_F1,                   /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F1) */
		KEY_F2                   = SDLK_F2,                   /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F2) */
		KEY_F3                   = SDLK_F3,                   /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F3) */
		KEY_F4                   = SDLK_F4,                   /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F4) */
		KEY_F5                   = SDLK_F5,                   /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F5) */
		KEY_F6                   = SDLK_F6,                   /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F6) */
		KEY_F7                   = SDLK_F7,                   /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F7) */
		KEY_F8                   = SDLK_F8,                   /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F8) */
		KEY_F9                   = SDLK_F9,                   /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F9) */
		KEY_F10                  = SDLK_F10,                  /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F10) */
		KEY_F11                  = SDLK_F11,                  /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F11) */
		KEY_F12                  = SDLK_F12,                  /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F12) */
		KEY_PRINTSCREEN          = SDLK_PRINTSCREEN,          /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PRINTSCREEN) */
		KEY_SCROLLLOCK           = SDLK_SCROLLLOCK,           /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SCROLLLOCK) */
		KEY_PAUSE                = SDLK_PAUSE,                /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAUSE) */
		KEY_INSERT               = SDLK_INSERT,               /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_INSERT) */
		KEY_HOME                 = SDLK_HOME,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_HOME) */
		KEY_PAGEUP               = SDLK_PAGEUP,               /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAGEUP) */
		KEY_END                  = SDLK_END,                  /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_END) */
		KEY_PAGEDOWN             = SDLK_PAGEDOWN,             /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAGEDOWN) */
		KEY_RIGHT                = SDLK_RIGHT,                /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RIGHT) */
		KEY_LEFT                 = SDLK_LEFT,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LEFT) */
		KEY_DOWN                 = SDLK_DOWN,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DOWN) */
		KEY_UP                   = SDLK_UP,                   /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_UP) */
		KEY_NUMLOCKCLEAR         = SDLK_NUMLOCKCLEAR,         /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMLOCKCLEAR) */
		KEY_KP_DIVIDE            = SDLK_KP_DIVIDE,            /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DIVIDE) */
		KEY_KP_MULTIPLY          = SDLK_KP_MULTIPLY,          /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MULTIPLY) */
		KEY_KP_MINUS             = SDLK_KP_MINUS,             /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MINUS) */
		KEY_KP_PLUS              = SDLK_KP_PLUS,              /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PLUS) */
		KEY_KP_ENTER             = SDLK_KP_ENTER,             /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_ENTER) */
		KEY_KP_1                 = SDLK_KP_1,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_1) */
		KEY_KP_2                 = SDLK_KP_2,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_2) */
		KEY_KP_3                 = SDLK_KP_3,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_3) */
		KEY_KP_4                 = SDLK_KP_4,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_4) */
		KEY_KP_5                 = SDLK_KP_5,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_5) */
		KEY_KP_6                 = SDLK_KP_6,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_6) */
		KEY_KP_7                 = SDLK_KP_7,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_7) */
		KEY_KP_8                 = SDLK_KP_8,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_8) */
		KEY_KP_9                 = SDLK_KP_9,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_9) */
		KEY_KP_0                 = SDLK_KP_0,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_0) */
		KEY_KP_PERIOD            = SDLK_KP_PERIOD,            /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PERIOD) */
		KEY_APPLICATION          = SDLK_APPLICATION,          /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_APPLICATION) */
		KEY_POWER                = SDLK_POWER,                /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_POWER) */
		KEY_KP_EQUALS            = SDLK_KP_EQUALS,            /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EQUALS) */
		KEY_F13                  = SDLK_F13,                  /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F13) */
		KEY_F14                  = SDLK_F14,                  /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F14) */
		KEY_F15                  = SDLK_F15,                  /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F15) */
		KEY_F16                  = SDLK_F16,                  /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F16) */
		KEY_F17                  = SDLK_F17,                  /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F17) */
		KEY_F18                  = SDLK_F18,                  /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F18) */
		KEY_F19                  = SDLK_F19,                  /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F19) */
		KEY_F20                  = SDLK_F20,                  /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F20) */
		KEY_F21                  = SDLK_F21,                  /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F21) */
		KEY_F22                  = SDLK_F22,                  /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F22) */
		KEY_F23                  = SDLK_F23,                  /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F23) */
		KEY_F24                  = SDLK_F24,                  /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F24) */
		KEY_EXECUTE              = SDLK_EXECUTE,              /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EXECUTE) */
		KEY_HELP                 = SDLK_HELP,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_HELP) */
		KEY_MENU                 = SDLK_MENU,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MENU) */
		KEY_SELECT               = SDLK_SELECT,               /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SELECT) */
		KEY_STOP                 = SDLK_STOP,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_STOP) */
		KEY_AGAIN                = SDLK_AGAIN,                /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AGAIN) */
		KEY_UNDO                 = SDLK_UNDO,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_UNDO) */
		KEY_CUT                  = SDLK_CUT,                  /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CUT) */
		KEY_COPY                 = SDLK_COPY,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_COPY) */
		KEY_PASTE                = SDLK_PASTE,                /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PASTE) */
		KEY_FIND                 = SDLK_FIND,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_FIND) */
		KEY_MUTE                 = SDLK_MUTE,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MUTE) */
		KEY_VOLUMEUP             = SDLK_VOLUMEUP,             /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_VOLUMEUP) */
		KEY_VOLUMEDOWN           = SDLK_VOLUMEDOWN,           /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_VOLUMEDOWN) */
		KEY_KP_COMMA             = SDLK_KP_COMMA,             /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_COMMA) */
		KEY_KP_EQUALSAS400       = SDLK_KP_EQUALSAS400,       /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EQUALSAS400) */
		KEY_ALTERASE             = SDLK_ALTERASE,             /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_ALTERASE) */
		KEY_SYSREQ               = SDLK_SYSREQ,               /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SYSREQ) */
		KEY_CANCEL               = SDLK_CANCEL,               /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CANCEL) */
		KEY_CLEAR                = SDLK_CLEAR,                /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CLEAR) */
		KEY_PRIOR                = SDLK_PRIOR,                /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PRIOR) */
		KEY_RETURN2              = SDLK_RETURN2,              /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RETURN2) */
		KEY_SEPARATOR            = SDLK_SEPARATOR,            /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SEPARATOR) */
		KEY_OUT                  = SDLK_OUT,                  /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_OUT) */
		KEY_OPER                 = SDLK_OPER,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_OPER) */
		KEY_CLEARAGAIN           = SDLK_CLEARAGAIN,           /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CLEARAGAIN) */
		KEY_CRSEL                = SDLK_CRSEL,                /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CRSEL) */
		KEY_EXSEL                = SDLK_EXSEL,                /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EXSEL) */
		KEY_KP_00                = SDLK_KP_00,                /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_00) */
		KEY_KP_000               = SDLK_KP_000,               /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_000) */
		KEY_THOUSANDSSEPARATOR   = SDLK_THOUSANDSSEPARATOR,   /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_THOUSANDSSEPARATOR) */
		KEY_DECIMALSEPARATOR     = SDLK_DECIMALSEPARATOR,     /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DECIMALSEPARATOR) */
		KEY_CURRENCYUNIT         = SDLK_CURRENCYUNIT,         /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CURRENCYUNIT) */
		KEY_CURRENCYSUBUNIT      = SDLK_CURRENCYSUBUNIT,      /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CURRENCYSUBUNIT) */
		KEY_KP_LEFTPAREN         = SDLK_KP_LEFTPAREN,         /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LEFTPAREN) */
		KEY_KP_RIGHTPAREN        = SDLK_KP_RIGHTPAREN,        /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_RIGHTPAREN) */
		KEY_KP_LEFTBRACE         = SDLK_KP_LEFTBRACE,         /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LEFTBRACE) */
		KEY_KP_RIGHTBRACE        = SDLK_KP_RIGHTBRACE,        /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_RIGHTBRACE) */
		KEY_KP_TAB               = SDLK_KP_TAB,               /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_TAB) */
		KEY_KP_BACKSPACE         = SDLK_KP_BACKSPACE,         /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_BACKSPACE) */
		KEY_KP_A                 = SDLK_KP_A,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_A) */
		KEY_KP_B                 = SDLK_KP_B,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_B) */
		KEY_KP_C                 = SDLK_KP_C,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_C) */
		KEY_KP_D                 = SDLK_KP_D,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_D) */
		KEY_KP_E                 = SDLK_KP_E,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_E) */
		KEY_KP_F                 = SDLK_KP_F,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_F) */
		KEY_KP_XOR               = SDLK_KP_XOR,               /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_XOR) */
		KEY_KP_POWER             = SDLK_KP_POWER,             /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_POWER) */
		KEY_KP_PERCENT           = SDLK_KP_PERCENT,           /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PERCENT) */
		KEY_KP_LESS              = SDLK_KP_LESS,              /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LESS) */
		KEY_KP_GREATER           = SDLK_KP_GREATER,           /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_GREATER) */
		KEY_KP_AMPERSAND         = SDLK_KP_AMPERSAND,         /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_AMPERSAND) */
		KEY_KP_DBLAMPERSAND      = SDLK_KP_DBLAMPERSAND,      /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DBLAMPERSAND) */
		KEY_KP_VERTICALBAR       = SDLK_KP_VERTICALBAR,       /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_VERTICALBAR) */
		KEY_KP_DBLVERTICALBAR    = SDLK_KP_DBLVERTICALBAR,    /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DBLVERTICALBAR) */
		KEY_KP_COLON             = SDLK_KP_COLON,             /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_COLON) */
		KEY_KP_HASH              = SDLK_KP_HASH,              /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_HASH) */
		KEY_KP_SPACE             = SDLK_KP_SPACE,             /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_SPACE) */
		KEY_KP_AT                = SDLK_KP_AT,                /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_AT) */
		KEY_KP_EXCLAM            = SDLK_KP_EXCLAM,            /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EXCLAM) */
		KEY_KP_MEMSTORE          = SDLK_KP_MEMSTORE,          /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMSTORE) */
		KEY_KP_MEMRECALL         = SDLK_KP_MEMRECALL,         /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMRECALL) */
		KEY_KP_MEMCLEAR          = SDLK_KP_MEMCLEAR,          /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMCLEAR) */
		KEY_KP_MEMADD            = SDLK_KP_MEMADD,            /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMADD) */
		KEY_KP_MEMSUBTRACT       = SDLK_KP_MEMSUBTRACT,       /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMSUBTRACT) */
		KEY_KP_MEMMULTIPLY       = SDLK_KP_MEMMULTIPLY,       /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMMULTIPLY) */
		KEY_KP_MEMDIVIDE         = SDLK_KP_MEMDIVIDE,         /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMDIVIDE) */
		KEY_KP_PLUSMINUS         = SDLK_KP_PLUSMINUS,         /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PLUSMINUS) */
		KEY_KP_CLEAR             = SDLK_KP_CLEAR,             /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_CLEAR) */
		KEY_KP_CLEARENTRY        = SDLK_KP_CLEARENTRY,        /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_CLEARENTRY) */
		KEY_KP_BINARY            = SDLK_KP_BINARY,            /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_BINARY) */
		KEY_KP_OCTAL             = SDLK_KP_OCTAL,             /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_OCTAL) */
		KEY_KP_DECIMAL           = SDLK_KP_DECIMAL,           /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DECIMAL) */
		KEY_KP_HEXADECIMAL       = SDLK_KP_HEXADECIMAL,       /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_HEXADECIMAL) */
		KEY_LCTRL                = SDLK_LCTRL,                /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LCTRL) */
		KEY_LSHIFT               = SDLK_LSHIFT,               /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LSHIFT) */
		KEY_LALT                 = SDLK_LALT,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LALT) */
		KEY_LGUI                 = SDLK_LGUI,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LGUI) */
		KEY_RCTRL                = SDLK_RCTRL,                /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RCTRL) */
		KEY_RSHIFT               = SDLK_RSHIFT,               /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RSHIFT) */
		KEY_RALT                 = SDLK_RALT,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RALT) */
		KEY_RGUI                 = SDLK_RGUI,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RGUI) */
		KEY_MODE                 = SDLK_MODE,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MODE) */
		KEY_SLEEP                = SDLK_SLEEP,                /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SLEEP) */
		KEY_WAKE                 = SDLK_WAKE,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_WAKE) */
		KEY_CHANNEL_INCREMENT    = SDLK_CHANNEL_INCREMENT,    /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CHANNEL_INCREMENT) */
		KEY_CHANNEL_DECREMENT    = SDLK_CHANNEL_DECREMENT,    /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CHANNEL_DECREMENT) */
		KEY_MEDIA_PLAY           = SDLK_MEDIA_PLAY,           /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_PLAY) */
		KEY_MEDIA_PAUSE          = SDLK_MEDIA_PAUSE,          /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_PAUSE) */
		KEY_MEDIA_RECORD         = SDLK_MEDIA_RECORD,         /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_RECORD) */
		KEY_MEDIA_FAST_FORWARD   = SDLK_MEDIA_FAST_FORWARD,   /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_FAST_FORWARD) */
		KEY_MEDIA_REWIND         = SDLK_MEDIA_REWIND,         /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_REWIND) */
		KEY_MEDIA_NEXT_TRACK     = SDLK_MEDIA_NEXT_TRACK,     /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_NEXT_TRACK) */
		KEY_MEDIA_PREVIOUS_TRACK = SDLK_MEDIA_PREVIOUS_TRACK, /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_PREVIOUS_TRACK) */
		KEY_MEDIA_STOP           = SDLK_MEDIA_STOP,           /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_STOP) */
		KEY_MEDIA_EJECT          = SDLK_MEDIA_EJECT,          /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_EJECT) */
		KEY_MEDIA_PLAY_PAUSE     = SDLK_MEDIA_PLAY_PAUSE,     /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_PLAY_PAUSE) */
		KEY_MEDIA_SELECT         = SDLK_MEDIA_SELECT,         /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_SELECT) */
		KEY_AC_NEW               = SDLK_AC_NEW,               /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_NEW) */
		KEY_AC_OPEN              = SDLK_AC_OPEN,              /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_OPEN) */
		KEY_AC_CLOSE             = SDLK_AC_CLOSE,             /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_CLOSE) */
		KEY_AC_EXIT              = SDLK_AC_EXIT,              /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_EXIT) */
		KEY_AC_SAVE              = SDLK_AC_SAVE,              /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_SAVE) */
		KEY_AC_PRINT             = SDLK_AC_PRINT,             /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_PRINT) */
		KEY_AC_PROPERTIES        = SDLK_AC_PROPERTIES,        /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_PROPERTIES) */
		KEY_AC_SEARCH            = SDLK_AC_SEARCH,            /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_SEARCH) */
		KEY_AC_HOME              = SDLK_AC_HOME,              /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_HOME) */
		KEY_AC_BACK              = SDLK_AC_BACK,              /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_BACK) */
		KEY_AC_FORWARD           = SDLK_AC_FORWARD,           /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_FORWARD) */
		KEY_AC_STOP              = SDLK_AC_STOP,              /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_STOP) */
		KEY_AC_REFRESH           = SDLK_AC_REFRESH,           /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_REFRESH) */
		KEY_AC_BOOKMARKS         = SDLK_AC_BOOKMARKS,         /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_BOOKMARKS) */
		KEY_SOFTLEFT             = SDLK_SOFTLEFT,             /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SOFTLEFT) */
		KEY_SOFTRIGHT            = SDLK_SOFTRIGHT,            /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SOFTRIGHT) */
		KEY_CALL                 = SDLK_CALL,                 /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CALL) */
		KEY_ENDCALL              = SDLK_ENDCALL,              /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_ENDCALL) */
		KEY_LEFT_TAB             = SDLK_LEFT_TAB,             /**< Extended key Left Tab */
		KEY_LEVEL5_SHIFT         = SDLK_LEVEL5_SHIFT,         /**< Extended key Level 5 Shift */
		KEY_MULTI_KEY_COMPOSE    = SDLK_MULTI_KEY_COMPOSE,    /**< Extended key Multi-key Compose */
		KEY_LMETA                = SDLK_LMETA,                /**< Extended key Left Meta */
		KEY_RMETA                = SDLK_RMETA,                /**< Extended key Right Meta */
		KEY_LHYPER               = SDLK_LHYPER,               /**< Extended key Left Hyper */
		KEY_RHYPER               = SDLK_RHYPER                /**< Extended key Right Hyper */
	};

	///
	/// Enum class for key modifiers.
	///
	enum class KeyMods : SDL_Keymod
	{
		MOD_NONE   = SDL_KMOD_NONE,   /**< no modifier is applicable. */
		MOD_LSHIFT = SDL_KMOD_LSHIFT, /**< the left Shift key is down. */
		MOD_RSHIFT = SDL_KMOD_RSHIFT, /**< the right Shift key is down. */
		MOD_LEVEL5 = SDL_KMOD_LEVEL5, /**< the Level 5 Shift key is down. */
		MOD_LCTRL  = SDL_KMOD_LCTRL,  /**< the left Ctrl (Control) key is down. */
		MOD_RCTRL  = SDL_KMOD_RCTRL,  /**< the right Ctrl (Control) key is down. */
		MOD_LALT   = SDL_KMOD_LALT,   /**< the left Alt key is down. */
		MOD_RALT   = SDL_KMOD_RALT,   /**< the right Alt key is down. */
		MOD_LGUI   = SDL_KMOD_LGUI,   /**< the left GUI key (often the Windows key) is down. */
		MOD_RGUI   = SDL_KMOD_RGUI,   /**< the right GUI key (often the Windows key) is down. */
		MOD_NUM    = SDL_KMOD_NUM,    /**< the Num Lock key (may be located on an extended keypad) is down. */
		MOD_CAPS   = SDL_KMOD_CAPS,   /**< the Caps Lock key is down. */
		MOD_MODE   = SDL_KMOD_MODE,   /**< the !AltGr key is down. */
		MOD_SCROLL = SDL_KMOD_SCROLL, /**< the Scroll Lock key is down. */
		MOD_CTRL   = SDL_KMOD_CTRL,   /**< Any Ctrl key is down. */
		MOD_SHIFT  = SDL_KMOD_SHIFT,  /**< Any Shift key is down. */
		MOD_ALT    = SDL_KMOD_ALT,    /**< Any Alt key is down. */
		MOD_GUI    = SDL_KMOD_GUI     /**< Any GUI key is down. */
	};
} // namespace galaxy

#endif
