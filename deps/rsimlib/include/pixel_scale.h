#pragma once

#include "display.h"
#include "draw.h"
#include "input.h"

#include <algorithm>
#include <cmath>

namespace sl
{
    /** How a fixed-size game image is fitted into a window of a different size. */
    enum class ScaleMode
    {
        /** Fill the whole window; the aspect ratio is not preserved. */
        stretch,
        /** Largest uniform scale that fits, with bars on the unused sides. */
        aspect_fit,
        /** Largest whole-number scale that fits (at least 1), so every game pixel is the same size. */
        integer
    };

    /** The window rectangle a fixed-size image is drawn into, and the scale applied to it. */
    struct ScaleFit
    {
        int x = 0;
        int y = 0;
        int width = 0;
        int height = 0;
        float scale_x = 1.0f;
        float scale_y = 1.0f;

        /** Convert a window position (for example the mouse) to game-image coordinates. */
        void window_to_game(float window_x, float window_y, float &game_x, float &game_y) const
        {
            game_x = scale_x > 0.0f ? (window_x - x) / scale_x : window_x;
            game_y = scale_y > 0.0f ? (window_y - y) / scale_y : window_y;
        }
        /** Convert game-image coordinates to window coordinates. */
        void game_to_window(float game_x, float game_y, float &window_x, float &window_y) const
        {
            window_x = x + game_x * scale_x;
            window_y = y + game_y * scale_y;
        }
        /** True when a window position falls inside the image rather than on a bar. */
        bool contains(float window_x, float window_y) const
        {
            return window_x >= x && window_y >= y && window_x < x + width && window_y < y + height;
        }
    };

    /** Compute where a game_width x game_height image goes inside a window_width x window_height window, centred. */
    inline ScaleFit compute_scale_fit(int game_width, int game_height, int window_width, int window_height,
                                      ScaleMode mode = ScaleMode::integer)
    {
        ScaleFit fit;
        if (game_width <= 0 || game_height <= 0 || window_width <= 0 || window_height <= 0)
            return fit;
        if (mode == ScaleMode::stretch)
        {
            fit.width = window_width;
            fit.height = window_height;
        }
        else
        {
            float scale = std::min(static_cast<float>(window_width) / game_width,
                                   static_cast<float>(window_height) / game_height);
            if (mode == ScaleMode::integer)
                scale = std::max(1.0f, std::floor(scale));
            fit.width = static_cast<int>(std::lround(game_width * scale));
            fit.height = static_cast<int>(std::lround(game_height * scale));
        }
        fit.x = (window_width - fit.width) / 2;
        fit.y = (window_height - fit.height) / 2;
        fit.scale_x = static_cast<float>(fit.width) / game_width;
        fit.scale_y = static_cast<float>(fit.height) / game_height;
        return fit;
    }

    /**
     * Draw a fixed-size render target (see create_render_target()) onto the screen according to a ScaleFit.
     * Call after end_render_target(). The target is sampled with nearest filtering unless its `smooth` flag was set
     * before it was first used. The bars are whatever the screen was cleared to.
     */
    inline void draw_scaled_target(Bitmap *target, const ScaleFit &fit)
    {
        if (!target || fit.width <= 0 || fit.height <= 0)
            return;
        stretch_blit(target, screen, 0, 0, target->width, target->height,
                     static_cast<float>(fit.x), static_cast<float>(fit.y), fit.width, fit.height);
    }

    /** Make mouse_x()/mouse_y() report coordinates inside the scaled game image (see set_mouse_mapping()). */
    inline void set_mouse_scale_fit(const ScaleFit &fit)
    {
        set_mouse_mapping(fit.x, fit.y, fit.scale_x, fit.scale_y);
    }

    /**
     * Fit a render target to the current screen size, draw it and route the mouse to its coordinates. Call every
     * frame after end_render_target(); it follows window resizes and fullscreen toggles because the fit is recomputed
     * from screen_width()/screen_height() each time. Returns the fit used.
     */
    inline ScaleFit present_scaled_target(Bitmap *target, ScaleMode mode = ScaleMode::integer)
    {
        ScaleFit fit;
        if (!target)
            return fit;
        fit = compute_scale_fit(target->width, target->height, screen_width(), screen_height(), mode);
        draw_scaled_target(target, fit);
        set_mouse_scale_fit(fit);
        return fit;
    }
}
