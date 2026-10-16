#pragma once

#include <SDL2/SDL_scancode.h>

#include "event.h"

#include <cstdint>

namespace sl
{
    using Key = Event::Key;

    /** Identifiers for mouse buttons. */
    enum class MouseButton : std::uint8_t
    {
        left = 1,
        middle = 2,
        right = 3,
        x1 = 4,
        x2 = 5
    };

    /** Bitmask constants matching mouse_buttons(). */
    constexpr std::uint32_t MOUSE_BUTTON_LEFT_MASK = 1u << 0;
    constexpr std::uint32_t MOUSE_BUTTON_MIDDLE_MASK = 1u << 1;
    constexpr std::uint32_t MOUSE_BUTTON_RIGHT_MASK = 1u << 2;

    /** Poll the next SDL event, returning false when the queue is empty. */
    bool poll_event(Event *event);
    /** Return whether a keyboard scancode is currently held. */
    bool key_down(SDL_Scancode key);
    bool key_down(Event::Key key);
    /** Return the current mouse X coordinate in window pixels. */
    int mouse_x();
    /** Return the current mouse Y coordinate in window pixels. */
    int mouse_y();
    /** Return the current mouse button bitmask. */
    std::uint32_t mouse_buttons();
    /** Return whether a mouse button is currently held down. */
    bool mouse_button_down(MouseButton button);
    bool mouse_button_down(int button);

    /** Enable or disable relative mouse mode (locks cursor to window, hides it, and reports relative motion). */
    bool set_relative_mouse_mode(bool enabled);
    /** Return whether relative mouse mode is currently enabled. */
    bool relative_mouse_mode();

    /** Retrieve relative mouse movement (delta) since the last call. */
    void mouse_delta(int *dx, int *dy);
    inline void mouse_delta(int &dx, int &dy)
    {
        mouse_delta(&dx, &dy);
    }
    /** Retrieve relative mouse movement and return the held button bitmask. */
    std::uint32_t mouse_relative_state(int *dx, int *dy = nullptr);

} // namespace sl