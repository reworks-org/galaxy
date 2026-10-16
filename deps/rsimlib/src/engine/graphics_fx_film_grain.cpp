/** @file
 * @brief Implements the FilmGrain post-process effect.
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

    bool FilmGrain::initialise()
    {
        if (is_valid())
            return true;
        return shader_.load(load_glsl_shader("fullscreen.vert"), load_glsl_shader("film_grain.frag"),
                            "film-grain");
    }
    void FilmGrain::shutdown() { shader_.reset(); }
    bool FilmGrain::is_valid() const { return shader_.is_valid(); }
    const std::string &FilmGrain::error() const { return shader_.error(); }
    void FilmGrain::set_strength(float strength) { strength_ = std::max(0.0f, strength); }
    void FilmGrain::set_time(float time) { time_ = time; }

    void FilmGrain::apply(Bitmap *source, int x, int y, int width, int height) const
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
        shader_.set_uniform("strength", strength_);
        shader_.set_uniform("time", time_);
        float projection[16];
        detail::gl2d_ortho_matrix(screen_width(), screen_height(), projection);
        shader_.set_uniform_mat4("uProjection", projection);
        submit_fullscreen_quad(x, y, width, height, source->gpu_texture, flip_vertical);
        Shader::stop();
    }

} // namespace sl