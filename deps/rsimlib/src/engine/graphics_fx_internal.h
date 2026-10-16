/** @file
 * @brief Shared helpers used by the individual graphics_fx_*.cpp effect implementations.
 */
#pragma once

#include "graphics_fx.h"

#include <SDL2/SDL_opengl.h>

#include <string>

namespace sl::detail
{
    /** Load the text contents of a built-in GLSL shader asset from disk. */
    std::string load_glsl_shader(const char *name);

    /** Draw a textured quad at (x, y, width, height), used by every fullscreen effect pass. */
    void submit_fullscreen_quad(int x, int y, int width, int height, GLuint texture,
                                bool flipVertical, bool premultipliedAlpha = true);
}
