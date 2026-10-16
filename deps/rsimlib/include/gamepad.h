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
    /** Return whether @p index identifies a connected gamepad. */
    bool gamepad_connected(int index);
    /** Return a connected gamepad's descriptive name, or an empty string for an invalid index. */
    std::string gamepad_name(int index);
    /** Return whether a standard button is currently held. */
    bool gamepad_button(int index, GamepadButton button);
    /** Return a normalized axis value after applying the configured dead zone. */
    float gamepad_axis(int index, GamepadAxis axis);
    /** Set the analogue dead zone in the range [0, 1). */
    void gamepad_set_deadzone(float deadzone);
    /** Return the analogue dead zone in use. */
    float gamepad_deadzone();

} // namespace sl