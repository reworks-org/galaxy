#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace sl
{

    /** A backend-independent event supplied by poll_event(). */
    class Event
    {
    public:
        /** Normalized category of the wrapped SDL event. */
        enum class Type
        {
            unknown,
            first,
            quit,
            app_terminating,
            app_low_memory,
            app_will_enter_background,
            app_did_enter_background,
            app_will_enter_foreground,
            app_did_enter_foreground,
            locale_changed,
            display,
            window,
            system_window_manager,
            key_down,
            key_up,
            text_editing,
            text_input,
            keymap_changed,
            text_editing_extended,
            mouse_motion,
            mouse_button_down,
            mouse_button_up,
            mouse_wheel,
            joystick_axis_motion,
            joystick_ball_motion,
            joystick_hat_motion,
            joystick_button_down,
            joystick_button_up,
            joystick_added,
            joystick_removed,
            joystick_battery_updated,
            gamepad_axis_motion,
            gamepad_button_down,
            gamepad_button_up,
            window_resized,
            gamepad_added,
            gamepad_removed,
            gamepad_remapped,
            gamepad_touchpad_down,
            gamepad_touchpad_motion,
            gamepad_touchpad_up,
            gamepad_sensor_update,
            gamepad_update_complete,
            gamepad_steam_handle_updated,
            finger_down,
            finger_up,
            finger_motion,
            dollar_gesture,
            dollar_record,
            multi_gesture,
            clipboard_updated,
            drop_file,
            drop_text,
            drop_begin,
            drop_complete,
            audio_device_added,
            audio_device_removed,
            sensor_update,
            render_targets_reset,
            render_device_reset,
            poll_sentinel,
            user,
            last
        };

        /** Keys currently normalized by simlib's event abstraction. */
        enum class Key
        {
            unknown,
            other,
            escape,
            space,
            return_key,
            keypad_enter,
            backspace,
            tab,
            exclaim,
            double_quote,
            hash,
            percent,
            dollar,
            ampersand,
            quote,
            left_parenthesis,
            right_parenthesis,
            asterisk,
            plus,
            comma,
            minus,
            period,
            slash,
            digit_0,
            letter_m,
            letter_s,
            digit_1,
            digit_2,
            digit_3,
            digit_4,
            digit_5,
            digit_6,
            digit_7,
            digit_8,
            digit_9,
            colon,
            semicolon,
            less,
            equals,
            greater,
            question,
            at,
            left_bracket,
            backslash,
            right_bracket,
            caret,
            underscore,
            backquote,
            letter_a,
            letter_b,
            letter_c,
            letter_d,
            letter_e,
            letter_f,
            letter_g,
            letter_h,
            letter_i,
            letter_j,
            letter_k,
            letter_l,
            letter_n,
            letter_o,
            letter_p,
            letter_q,
            letter_r,
            letter_t,
            letter_u,
            letter_v,
            letter_w,
            letter_x,
            letter_y,
            letter_z,
            caps_lock,
            f1,
            f2,
            f3,
            f4,
            f5,
            f6,
            f7,
            f8,
            f9,
            f10,
            f11,
            f12,
            print_screen,
            scroll_lock,
            pause,
            insert,
            home,
            page_up,
            delete_key,
            end,
            page_down,
            arrow_right,
            arrow_left,
            arrow_down,
            arrow_up,
            num_lock,
            keypad_divide,
            keypad_multiply,
            keypad_minus,
            keypad_plus,
            keypad_0,
            keypad_1,
            keypad_2,
            keypad_3,
            keypad_4,
            keypad_5,
            keypad_6,
            keypad_7,
            keypad_8,
            keypad_9,
            keypad_period,
            application,
            power,
            keypad_equals,
            left_control,
            left_shift,
            left_alt,
            left_gui,
            right_control,
            right_shift,
            right_alt,
            right_gui
        };

        /** Construct an event with unknown type and default data. */
        Event();
        /** Release the wrapped event data. */
        ~Event();
        /** Move an event and its backend data. */
        Event(Event &&) noexcept;
        /** Replace this event with the data from another event. */
        Event &operator=(Event &&) noexcept;
        Event(const Event &) = delete;
        Event &operator=(const Event &) = delete;

        /** Return the event category. */
        Type type() const;
        /** Return the normalized key for keyboard events. */
        Key key() const;
        /** Return the backend keycode for keyboard events, including keys represented by Key::other. */
        std::int32_t key_code() const;
        /** Return whether a keyboard event was generated by key repeat. */
        bool key_repeat() const;
        /** Return UTF-8 text for text-input events. */
        const std::string &text() const;
        /** Return the current width for a window-resized event. */
        int window_width() const;
        /** Return the current height for a window-resized event. */
        int window_height() const;
        /** Return the SDL mouse button value for mouse button events, or 0 otherwise. */
        std::uint8_t mouse_button() const;
        /** Return an SDL-compatible controller device index for an added gamepad. */
        int gamepad_device_index() const;
        /** Return an SDL-compatible controller instance ID for a removed gamepad. */
        int gamepad_instance_id() const;

    private:
        struct Implementation;
        std::unique_ptr<Implementation> implementation_;

        friend bool poll_event(Event *event);
        friend const void *detail_event_handle(const Event &event);
    };

    namespace detail
    {
        /** Return the native event only for backend integrations such as ImGui. */
        const void *event_handle(const Event &event);
    }

} // namespace sl