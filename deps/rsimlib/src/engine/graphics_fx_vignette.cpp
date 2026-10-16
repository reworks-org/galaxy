/** @file
 * @brief Implements the Vignette post-process effect.
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

	Vignette::~Vignette()
	{
		shutdown();
	}

	Vignette::Vignette(Vignette &&other) noexcept
		: shader_(std::move(other.shader_)),
		  radius_(other.radius_),
		  softness_(other.softness_),
		  intensity_(other.intensity_)
	{
	}

	Vignette &Vignette::operator=(Vignette &&other) noexcept
	{
		if (this != &other)
		{
			shader_ = std::move(other.shader_);
			radius_ = other.radius_;
			softness_ = other.softness_;
			intensity_ = other.intensity_;
		}
		return *this;
	}

	bool Vignette::initialise()
	{
		if (is_valid())
		{
			return true;
		}
		return shader_.load(load_glsl_shader("fullscreen.vert"), load_glsl_shader("vignette.frag"), "vignette");
	}

	void Vignette::shutdown()
	{
		shader_.reset();
	}

	bool Vignette::is_valid() const
	{
		return shader_.is_valid();
	}

	const std::string &Vignette::error() const
	{
		return shader_.error();
	}

	void Vignette::set_radius(float radius)
	{
		radius_ = std::clamp(radius, 0.0f, 1.0f);
	}

	void Vignette::set_softness(float softness)
	{
		softness_ = std::clamp(softness, 0.0001f, 1.0f);
	}

	void Vignette::set_intensity(float intensity)
	{
		intensity_ = std::clamp(intensity, 0.0f, 1.0f);
	}

	void Vignette::apply(Bitmap *source, int x, int y, int width, int height) const
	{
		const bool flipVertical = graphics_backend() == GraphicsBackend::opengl;
		if (!source || !is_valid() || !upload_bitmap(source))
		{
			return;
		}

		if (width <= 0)
		{
			width = screen_width();
		}
		if (height <= 0)
		{
			height = screen_height();
		}
		if (width <= 0 || height <= 0)
		{
			return;
		}

		float projection[16];
		shader_.set_uniform("source", 0);
		shader_.set_uniform("radius", radius_);
		shader_.set_uniform("softness", softness_);
		shader_.set_uniform("intensity", intensity_);
		detail::gl2d_ortho_matrix(screen_width(), screen_height(), projection);
		shader_.set_uniform_mat4("uProjection", projection);
		const bool sourceFlipVertical = graphics_backend() == GraphicsBackend::vulkan ? false : flipVertical;
		submit_fullscreen_quad(x, y, width, height, source->gpu_texture, sourceFlipVertical);
		Shader::stop();
	}

} // namespace sl
