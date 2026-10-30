#pragma once

#include "event.h"
#include <string>

namespace sl
{

    /** Buttons on a standard SDL game controller layout. */
    enum class GamepadButton
    {
        south,
        east,
        west,
        north,
        back,
        guide,
        start,
        left_stick,
        right_stick,
        left_shoulder,
        right_shoulder,
        dpad_up,
        dpad_down,
        dpad_left,
        dpad_right
    };

    /** Analogue inputs on a standard SDL game controller layout. */
    enum class GamepadAxis
    {
        left_x,
        left_y,
        right_x,
        right_y,
        left_trigger,
        right_trigger
    };

    /** Initialise SDL's game-controller subsystem and open connected mapped controllers. */
    bool gamepad_init();
    /** Close open controllers and release SDL's game-controller subsystem. */
    void gamepad_shutdown();
    /** Process SDL controller device events so connected devices stay current. */
    void gamepad_handle_event(const Event &event);
    /** Return the number of currently connected mapped gamepads. */
    int gamepad_count();
    /** Return whether @p index identifies a connected gamepad. Indices are positions in the connected list, so unplugging a device shifts the indices of later ones; re-query after gamepad_removed events. */
    bool gamepad_connected(int index);
    /** Return a connected gamepad's descriptive name, or an empty string for an invalid index. */
    std::string gamepad_name(int index);
    /** Return whether a standard button is currently held. */
    bool gamepad_button(int index, GamepadButton button);
    /** Return a normalized axis value after applying the configured dead zone. */
    float gamepad_axis(int index, GamepadAxis axis);
    /** Set the analogue dead zone in the range [0, 1). */
    /** Number of raw axes on a connected device (flight sticks, throttles and pedals expose axes that the gamepad layout does not name). */
    int joystick_axis_count(int index);
    /** Raw axis position, -1 to 1, with no dead zone and no remapping. */
    float joystick_axis_raw(int index, int axis);
    /** Number of raw buttons on a connected device. */
    int joystick_button_count(int index);
    /** True while this raw button is held. */
    bool joystick_button_raw(int index, int button);
    /** Number of hat switches on a connected device. */
    int joystick_hat_count(int index);
    /** Hat position as a bitmask: 1 up, 2 right, 4 down, 8 left. */
    int joystick_hat_raw(int index, int hat);

    void gamepad_set_deadzone(float deadzone);
    /** Return the analogue dead zone in use. */
    float gamepad_deadzone();

} // namespace sl