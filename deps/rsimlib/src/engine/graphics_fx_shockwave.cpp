/** @file
 * @brief Implements the Shockwave post-process effect.
 */
#include "graphics_fx.h"
#include "graphics_fx_internal.h"

#include "display.h"
#include "draw.h"
#include "gl2d.h"
#include "renderer.h"

#include <algorithm>

namespace sl
{
    using detail::load_glsl_shader;
    using detail::submit_fullscreen_quad;

    bool Shockwave::initialise()
    {
        if (is_valid())
            return true;
        return shader_.load(load_glsl_shader("fullscreen.vert"), load_glsl_shader("shockwave.frag"),
                            "shockwave");
    }
    void Shockwave::shutdown() { shader_.reset(); }
    bool Shockwave::is_valid() const { return shader_.is_valid(); }
    const std::string &Shockwave::error() const { return shader_.error(); }
    void Shockwave::set_centre(float x, float y)
    {
        centre_x_ = x;
        centre_y_ = y;
    }
    void Shockwave::set_radius(float radius) { radius_ = std::max(0.0f, radius); }
    void Shockwave::set_width(float width) { width_ = std::max(0.0001f, width); }
    void Shockwave::set_strength(float strength) { strength_ = std::max(0.0f, strength); }

    void Shockwave::apply(Bitmap *source, int x, int y, int width, int height) const
    {
        const bool flip_vertical = graphics_backend() == GraphicsBackend::opengl;
        if (!source || !is_valid() || !upload_bitmap(source))
            return;
        if (width <= 0)
            width = screen_width();
        if (height <= 0)
            height = screen_height();
        if (width <= 0 || height <= 0)
            return;
        shader_.set_uniform("source", 0);
        shader_.set_uniform("centre", centre_x_, centre_y_);
        shader_.set_uniform("radius", radius_);
        shader_.set_uniform("width", width_);
        shader_.set_uniform("strength", strength_);
        float projection[16];
        detail::gl2d_ortho_matrix(screen_width(), screen_height(), projection);
        shader_.set_uniform_mat4("uProjection", projection);
        submit_fullscreen_quad(x, y, width, height, source->gpu_texture, flip_vertical);
        Shader::stop();
    }

} // namespace sl