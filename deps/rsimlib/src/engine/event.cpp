/** @file
 * @brief Implements conversion from SDL events to the public event abstraction.
 */

#include "event.h"

#include <SDL2/SDL.h>

namespace sl
{
    namespace
    {

        Event::Type event_type(const SDL_Event &event)
        {
            switch (event.type)
            {
            case SDL_FIRSTEVENT:
                return Event::Type::first;
            case SDL_QUIT:
                return Event::Type::quit;
            case SDL_APP_TERMINATING:
                return Event::Type::app_terminating;
            case SDL_APP_LOWMEMORY:
                return Event::Type::app_low_memory;
            case SDL_APP_WILLENTERBACKGROUND:
                return Event::Type::app_will_enter_background;
            case SDL_APP_DIDENTERBACKGROUND:
                return Event::Type::app_did_enter_background;
            case SDL_APP_WILLENTERFOREGROUND:
                return Event::Type::app_will_enter_foreground;
            case SDL_APP_DIDENTERFOREGROUND:
                return Event::Type::app_did_enter_foreground;
            case SDL_LOCALECHANGED:
                return Event::Type::locale_changed;
            case SDL_DISPLAYEVENT:
                return Event::Type::display;
            case SDL_SYSWMEVENT:
                return Event::Type::system_window_manager;
            case SDL_KEYDOWN:
                return Event::Type::key_down;
            case SDL_KEYUP:
                return Event::Type::key_up;
            case SDL_TEXTEDITING:
                return Event::Type::text_editing;
            case SDL_TEXTINPUT:
                return Event::Type::text_input;
            case SDL_KEYMAPCHANGED:
                return Event::Type::keymap_changed;
            case SDL_TEXTEDITING_EXT:
                return Event::Type::text_editing_extended;
            case SDL_MOUSEMOTION:
                return Event::Type::mouse_motion;
            case SDL_MOUSEBUTTONDOWN:
                return Event::Type::mouse_button_down;
            case SDL_MOUSEBUTTONUP:
                return Event::Type::mouse_button_up;
            case SDL_MOUSEWHEEL:
                return Event::Type::mouse_wheel;
            case SDL_JOYAXISMOTION:
                return Event::Type::joystick_axis_motion;
            case SDL_JOYBALLMOTION:
                return Event::Type::joystick_ball_motion;
            case SDL_JOYHATMOTION:
                return Event::Type::joystick_hat_motion;
            case SDL_JOYBUTTONDOWN:
                return Event::Type::joystick_button_down;
            case SDL_JOYBUTTONUP:
                return Event::Type::joystick_button_up;
            case SDL_JOYDEVICEADDED:
                return Event::Type::joystick_added;
            case SDL_JOYDEVICEREMOVED:
                return Event::Type::joystick_removed;
            case SDL_JOYBATTERYUPDATED:
                return Event::Type::joystick_battery_updated;
            case SDL_CONTROLLERAXISMOTION:
                return Event::Type::gamepad_axis_motion;
            case SDL_CONTROLLERBUTTONDOWN:
                return Event::Type::gamepad_button_down;
            case SDL_CONTROLLERBUTTONUP:
                return Event::Type::gamepad_button_up;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_CLOSE)
                    return Event::Type::quit;
                return event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED ? Event::Type::window_resized : Event::Type::window;
            case SDL_CONTROLLERDEVICEADDED:
                return Event::Type::gamepad_added;
            case SDL_CONTROLLERDEVICEREMOVED:
                return Event::Type::gamepad_removed;
            case SDL_CONTROLLERDEVICEREMAPPED:
                return Event::Type::gamepad_remapped;
            case SDL_CONTROLLERTOUCHPADDOWN:
                return Event::Type::gamepad_touchpad_down;
            case SDL_CONTROLLERTOUCHPADMOTION:
                return Event::Type::gamepad_touchpad_motion;
            case SDL_CONTROLLERTOUCHPADUP:
                return Event::Type::gamepad_touchpad_up;
            case SDL_CONTROLLERSENSORUPDATE:
                return Event::Type::gamepad_sensor_update;
            case SDL_CONTROLLERUPDATECOMPLETE_RESERVED_FOR_SDL3:
                return Event::Type::gamepad_update_complete;
            case SDL_CONTROLLERSTEAMHANDLEUPDATED:
                return Event::Type::gamepad_steam_handle_updated;
            case SDL_FINGERDOWN:
                return Event::Type::finger_down;
            case SDL_FINGERUP:
                return Event::Type::finger_up;
            case SDL_FINGERMOTION:
                return Event::Type::finger_motion;
            case SDL_DOLLARGESTURE:
                return Event::Type::dollar_gesture;
            case SDL_DOLLARRECORD:
                return Event::Type::dollar_record;
            case SDL_MULTIGESTURE:
                return Event::Type::multi_gesture;
            case SDL_CLIPBOARDUPDATE:
                return Event::Type::clipboard_updated;
            case SDL_DROPFILE:
                return Event::Type::drop_file;
            case SDL_DROPTEXT:
                return Event::Type::drop_text;
            case SDL_DROPBEGIN:
                return Event::Type::drop_begin;
            case SDL_DROPCOMPLETE:
                return Event::Type::drop_complete;
            case SDL_AUDIODEVICEADDED:
                return Event::Type::audio_device_added;
            case SDL_AUDIODEVICEREMOVED:
                return Event::Type::audio_device_removed;
            case SDL_SENSORUPDATE:
                return Event::Type::sensor_update;
            case SDL_RENDER_TARGETS_RESET:
                return Event::Type::render_targets_reset;
            case SDL_RENDER_DEVICE_RESET:
                return Event::Type::render_device_reset;
            case SDL_POLLSENTINEL:
                return Event::Type::poll_sentinel;
            case SDL_LASTEVENT:
                return Event::Type::last;
            default:
                return event.type >= SDL_USEREVENT && event.type < SDL_LASTEVENT ? Event::Type::user : Event::Type::unknown;
            }
        }

        Event::Key event_key(SDL_Keycode key)
        {
            switch (key)
            {
            case SDLK_ESCAPE:
                return Event::Key::escape;
            case SDLK_SPACE:
                return Event::Key::space;
            case SDLK_RETURN:
                return Event::Key::return_key;
            case SDLK_KP_ENTER:
                return Event::Key::keypad_enter;
            case SDLK_BACKSPACE:
                return Event::Key::backspace;
            case SDLK_TAB:
                return Event::Key::tab;
            case SDLK_EXCLAIM:
                return Event::Key::exclaim;
            case SDLK_QUOTEDBL:
                return Event::Key::double_quote;
            case SDLK_HASH:
                return Event::Key::hash;
            case SDLK_PERCENT:
                return Event::Key::percent;
            case SDLK_DOLLAR:
                return Event::Key::dollar;
            case SDLK_AMPERSAND:
                return Event::Key::ampersand;
            case SDLK_QUOTE:
                return Event::Key::quote;
            case SDLK_LEFTPAREN:
                return Event::Key::left_parenthesis;
            case SDLK_RIGHTPAREN:
                return Event::Key::right_parenthesis;
            case SDLK_ASTERISK:
                return Event::Key::asterisk;
            case SDLK_PLUS:
                return Event::Key::plus;
            case SDLK_COMMA:
                return Event::Key::comma;
            case SDLK_MINUS:
                return Event::Key::minus;
            case SDLK_PERIOD:
                return Event::Key::period;
            case SDLK_SLASH:
                return Event::Key::slash;
            case SDLK_0:
                return Event::Key::digit_0;
            case SDLK_5:
                return Event::Key::digit_5;
            case SDLK_6:
                return Event::Key::digit_6;
            case SDLK_7:
                return Event::Key::digit_7;
            case SDLK_8:
                return Event::Key::digit_8;
            case SDLK_9:
                return Event::Key::digit_9;
            case SDLK_COLON:
                return Event::Key::colon;
            case SDLK_SEMICOLON:
                return Event::Key::semicolon;
            case SDLK_LESS:
                return Event::Key::less;
            case SDLK_EQUALS:
                return Event::Key::equals;
            case SDLK_GREATER:
                return Event::Key::greater;
            case SDLK_QUESTION:
                return Event::Key::question;
            case SDLK_AT:
                return Event::Key::at;
            case SDLK_LEFTBRACKET:
                return Event::Key::left_bracket;
            case SDLK_BACKSLASH:
                return Event::Key::backslash;
            case SDLK_RIGHTBRACKET:
                return Event::Key::right_bracket;
            case SDLK_CARET:
                return Event::Key::caret;
            case SDLK_UNDERSCORE:
                return Event::Key::underscore;
            case SDLK_BACKQUOTE:
                return Event::Key::backquote;
            case SDLK_a:
                return Event::Key::letter_a;
            case SDLK_b:
                return Event::Key::letter_b;
            case SDLK_c:
                return Event::Key::letter_c;
            case SDLK_d:
                return Event::Key::letter_d;
            case SDLK_e:
                return Event::Key::letter_e;
            case SDLK_f:
                return Event::Key::letter_f;
            case SDLK_g:
                return Event::Key::letter_g;
            case SDLK_h:
                return Event::Key::letter_h;
            case SDLK_i:
                return Event::Key::letter_i;
            case SDLK_j:
                return Event::Key::letter_j;
            case SDLK_k:
                return Event::Key::letter_k;
            case SDLK_l:
                return Event::Key::letter_l;
            case SDLK_m:
                return Event::Key::letter_m;
            case SDLK_n:
                return Event::Key::letter_n;
            case SDLK_o:
                return Event::Key::letter_o;
            case SDLK_p:
                return Event::Key::letter_p;
            case SDLK_q:
                return Event::Key::letter_q;
            case SDLK_r:
                return Event::Key::letter_r;
            case SDLK_s:
                return Event::Key::letter_s;
            case SDLK_t:
                return Event::Key::letter_t;
            case SDLK_u:
                return Event::Key::letter_u;
            case SDLK_v:
                return Event::Key::letter_v;
            case SDLK_w:
                return Event::Key::letter_w;
            case SDLK_x:
                return Event::Key::letter_x;
            case SDLK_y:
                return Event::Key::letter_y;
            case SDLK_z:
                return Event::Key::letter_z;
            case SDLK_1:
                return Event::Key::digit_1;
            case SDLK_2:
                return Event::Key::digit_2;
            case SDLK_3:
                return Event::Key::digit_3;
            case SDLK_4:
                return Event::Key::digit_4;
            case SDLK_CAPSLOCK:
                return Event::Key::caps_lock;
            case SDLK_F1:
                return Event::Key::f1;
            case SDLK_F2:
                return Event::Key::f2;
            case SDLK_F3:
                return Event::Key::f3;
            case SDLK_F4:
                return Event::Key::f4;
            case SDLK_F5:
                return Event::Key::f5;
            case SDLK_F6:
                return Event::Key::f6;
            case SDLK_F7:
                return Event::Key::f7;
            case SDLK_F8:
                return Event::Key::f8;
            case SDLK_F9:
                return Event::Key::f9;
            case SDLK_F10:
                return Event::Key::f10;
            case SDLK_F11:
                return Event::Key::f11;
            case SDLK_F12:
                return Event::Key::f12;
            case SDLK_PRINTSCREEN:
                return Event::Key::print_screen;
            case SDLK_SCROLLLOCK:
                return Event::Key::scroll_lock;
            case SDLK_PAUSE:
                return Event::Key::pause;
            case SDLK_INSERT:
                return Event::Key::insert;
            case SDLK_HOME:
                return Event::Key::home;
            case SDLK_PAGEUP:
                return Event::Key::page_up;
            case SDLK_DELETE:
                return Event::Key::delete_key;
            case SDLK_END:
                return Event::Key::end;
            case SDLK_PAGEDOWN:
                return Event::Key::page_down;
            case SDLK_RIGHT:
                return Event::Key::arrow_right;
            case SDLK_LEFT:
                return Event::Key::arrow_left;
            case SDLK_DOWN:
                return Event::Key::arrow_down;
            case SDLK_UP:
                return Event::Key::arrow_up;
            case SDLK_NUMLOCKCLEAR:
                return Event::Key::num_lock;
            case SDLK_KP_DIVIDE:
                return Event::Key::keypad_divide;
            case SDLK_KP_MULTIPLY:
                return Event::Key::keypad_multiply;
            case SDLK_KP_MINUS:
                return Event::Key::keypad_minus;
            case SDLK_KP_PLUS:
                return Event::Key::keypad_plus;
            case SDLK_KP_0:
                return Event::Key::keypad_0;
            case SDLK_KP_1:
                return Event::Key::keypad_1;
            case SDLK_KP_2:
                return Event::Key::keypad_2;
            case SDLK_KP_3:
                return Event::Key::keypad_3;
            case SDLK_KP_4:
                return Event::Key::keypad_4;
            case SDLK_KP_5:
                return Event::Key::keypad_5;
            case SDLK_KP_6:
                return Event::Key::keypad_6;
            case SDLK_KP_7:
                return Event::Key::keypad_7;
            case SDLK_KP_8:
                return Event::Key::keypad_8;
            case SDLK_KP_9:
                return Event::Key::keypad_9;
            case SDLK_KP_PERIOD:
                return Event::Key::keypad_period;
            case SDLK_APPLICATION:
                return Event::Key::application;
            case SDLK_POWER:
                return Event::Key::power;
            case SDLK_KP_EQUALS:
                return Event::Key::keypad_equals;
            case SDLK_LCTRL:
                return Event::Key::left_control;
            case SDLK_LSHIFT:
                return Event::Key::left_shift;
            case SDLK_LALT:
                return Event::Key::left_alt;
            case SDLK_LGUI:
                return Event::Key::left_gui;
            case SDLK_RCTRL:
                return Event::Key::right_control;
            case SDLK_RSHIFT:
                return Event::Key::right_shift;
            case SDLK_RALT:
                return Event::Key::right_alt;
            case SDLK_RGUI:
                return Event::Key::right_gui;
            case SDLK_UNKNOWN:
                return Event::Key::unknown;
            default:
                return Event::Key::other;
            }
        }

    } // namespace

    struct Event::Implementation
    {
        SDL_Event native{};
        Type type = Type::unknown;
        Key key = Key::unknown;
        std::int32_t key_code = 0;
        bool repeat = false;
        std::string text;
        int width = 0;
        int height = 0;
        int gamepad_device_index = -1;
        int gamepad_instance_id = -1;
    };

    Event::Event() : implementation_(std::make_unique<Implementation>()) {}
    Event::~Event() = default;
    Event::Event(Event &&) noexcept = default;
    Event &Event::operator=(Event &&) noexcept = default;
    Event::Type Event::type() const { return implementation_->type; }
    Event::Key Event::key() const { return implementation_->key; }
    std::int32_t Event::key_code() const { return implementation_->key_code; }
    bool Event::key_repeat() const { return implementation_->repeat; }
    const std::string &Event::text() const { return implementation_->text; }
    int Event::window_width() const { return implementation_->width; }
    int Event::window_height() const { return implementation_->height; }
    std::uint8_t Event::mouse_button() const
    {
        return implementation_->type == Type::mouse_button_down || implementation_->type == Type::mouse_button_up
                   ? implementation_->native.button.button
                   : 0;
    }
    int Event::gamepad_device_index() const { return implementation_->gamepad_device_index; }
    int Event::gamepad_instance_id() const { return implementation_->gamepad_instance_id; }

    bool poll_event(Event *event)
    {
        if (!event || SDL_PollEvent(&event->implementation_->native) == 0)
            return false;
        const SDL_Event &native = event->implementation_->native;
        event->implementation_->type = event_type(native);
        event->implementation_->key = native.type == SDL_KEYDOWN || native.type == SDL_KEYUP ? event_key(native.key.keysym.sym) : Event::Key::unknown;
        event->implementation_->key_code = native.type == SDL_KEYDOWN || native.type == SDL_KEYUP ? native.key.keysym.sym : 0;
        event->implementation_->repeat = (native.type == SDL_KEYDOWN || native.type == SDL_KEYUP) && native.key.repeat != 0;
        event->implementation_->text = native.type == SDL_TEXTINPUT ? native.text.text : "";
        event->implementation_->width = native.type == SDL_WINDOWEVENT ? native.window.data1 : 0;
        event->implementation_->height = native.type == SDL_WINDOWEVENT ? native.window.data2 : 0;
        event->implementation_->gamepad_device_index = native.type == SDL_CONTROLLERDEVICEADDED ? native.cdevice.which : native.type == SDL_JOYDEVICEADDED ? native.jdevice.which
                                                                                                                                                           : -1;
        event->implementation_->gamepad_instance_id = native.type == SDL_CONTROLLERDEVICEREMOVED ? native.cdevice.which : native.type == SDL_JOYDEVICEREMOVED ? native.jdevice.which
                                                                                                                                                              : -1;
        return true;
    }

    const void *detail_event_handle(const Event &event) { return &event.implementation_->native; }

    namespace detail
    {
        const void *event_handle(const Event &event) { return detail_event_handle(event); }
    } // namespace detail

} // namespace sl