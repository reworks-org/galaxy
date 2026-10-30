#pragma once

#include <SDL3/SDL_video.h>
#include <cstdint>

namespace sl
{
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

    /** Return the current mouse X coordinate in window pixels. */
    int mouse_x();
    /** Return the current mouse Y coordinate in window pixels. */
    int mouse_y();
    /**
     * Make mouse_x()/mouse_y() report positions inside a scaled image drawn at (x, y) in screen coordinates with the
     * given scale, for example a letterboxed or integer-scaled render target. See present_scaled_target().
     * Positions over the bars fall outside the image's coordinate range.
     */
    void set_mouse_mapping(int x, int y, float scale_x, float scale_y);
    /** Return mouse_x()/mouse_y() to plain screen coordinates. */
    void clear_mouse_mapping();
    /** Return the current mouse button bitmask. */
    std::uint32_t mouse_buttons();
    /** Return whether a mouse button is currently held down. */
    bool mouse_button_down(MouseButton button);
    bool mouse_button_down(int button);

    /** Enable or disable relative mouse mode (locks cursor to window, hides it, and reports relative motion). */
	bool set_relative_mouse_mode(SDL_Window* window, bool enabled);
    /** Return whether relative mouse mode is currently enabled. */
	bool relative_mouse_mode(SDL_Window* window);

    /** Retrieve relative mouse movement (delta) since the last call. */
    void mouse_delta(int *dx, int *dy);
    inline void mouse_delta(int &dx, int &dy)
    {
        mouse_delta(&dx, &dy);
    }
    /** Retrieve relative mouse movement and return the held button bitmask. */
    std::uint32_t mouse_relative_state(int *dx, int *dy = nullptr);

} // namespace sl