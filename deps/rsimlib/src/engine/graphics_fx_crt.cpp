/** @file
 * @brief Implements the CRTFilter post-process effect.
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

	bool CRTFilter::initialise()
	{
		if (is_valid())
			return true;
		return shader_.load(load_glsl_shader("fullscreen.vert"), load_glsl_shader("crt.frag"), "crt");
	}
	void CRTFilter::shutdown() { shader_.reset(); }
	bool CRTFilter::is_valid() const { return shader_.is_valid(); }
	const std::string &CRTFilter::error() const { return shader_.error(); }
	void CRTFilter::set_pixel_size(float size) { pixel_size_ = std::max(1.0f, size); }
	void CRTFilter::set_scanline_strength(float strength) { scanline_strength_ = std::clamp(strength, 0.0f, 1.0f); }
	void CRTFilter::set_curvature(float curvature) { curvature_ = std::clamp(curvature, 0.0f, 1.0f); }

	void CRTFilter::apply(Bitmap *source, int x, int y, int width, int height) const
	{
		const bool flipVertical = graphics_backend() == GraphicsBackend::opengl;
		if (!source || !is_valid() || !upload_bitmap(source))
			return;
		if (width <= 0)
			width = screen_width();
		if (height <= 0)
			height = screen_height();
		if (width <= 0 || height <= 0)
			return;
		shader_.set_uniform("source", 0);
		shader_.set_uniform("resolution", static_cast<float>(width), static_cast<float>(height));
		shader_.set_uniform("pixelSize", pixel_size_);
		shader_.set_uniform("scanlineStrength", scanline_strength_);
		shader_.set_uniform("curvature", curvature_);
		float projection[16];
		detail::gl2d_ortho_matrix(screen_width(), screen_height(), projection);
		shader_.set_uniform_mat4("uProjection", projection);
		submit_fullscreen_quad(x, y, width, height, source->gpu_texture, flipVertical);
		Shader::stop();
	}

} // namespace sl
