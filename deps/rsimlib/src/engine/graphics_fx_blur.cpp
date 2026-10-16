/** @file
 * @brief Implements the Blur post-process effect.
 */
#include "graphics_fx.h"
#include "graphics_fx_internal.h"

#include "display.h"
#include "draw.h"
#include "gl2d.h"
#include "renderer.h"

#include <algorithm>
#include <utility>

namespace sl
{
	using detail::load_glsl_shader;
	using detail::submit_fullscreen_quad;

	Blur::~Blur() { shutdown(); }

	Blur::Blur(Blur &&other) noexcept
		: shader_(std::move(other.shader_)), radius_(other.radius_), iterations_(other.iterations_),
		  target_a_(std::exchange(other.target_a_, nullptr)), target_b_(std::exchange(other.target_b_, nullptr)) {}

	Blur &Blur::operator=(Blur &&other) noexcept
	{
		if (this != &other)
		{
			shutdown();
			shader_ = std::move(other.shader_);
			radius_ = other.radius_;
			iterations_ = other.iterations_;
			target_a_ = std::exchange(other.target_a_, nullptr);
			target_b_ = std::exchange(other.target_b_, nullptr);
		}
		return *this;
	}

	bool Blur::initialise()
	{
		if (is_valid())
			return true;
		return shader_.load(load_glsl_shader("fullscreen.vert"), load_glsl_shader("blur.frag"), "bloom-blur");
	}

	void Blur::shutdown()
	{
		destroy_bitmap(target_a_);
		destroy_bitmap(target_b_);
		target_a_ = nullptr;
		target_b_ = nullptr;
		shader_.reset();
	}

	bool Blur::is_valid() const { return shader_.is_valid(); }
	const std::string &Blur::error() const { return shader_.error(); }
	void Blur::set_radius(float radius) { radius_ = std::max(0.0f, radius); }
	void Blur::set_iterations(int iterations) { iterations_ = std::clamp(iterations, 1, 8); }
	void Blur::set_opacity(float opacity) { opacity_ = std::clamp(opacity, 0.0f, 16.0f); }

	bool Blur::ensure_targets(int width, int height) const
	{
		if (target_a_ && target_b_ && target_a_->width == width && target_a_->height == height)
			return true;
		destroy_bitmap(target_a_);
		destroy_bitmap(target_b_);
		target_a_ = create_render_target(width, height);
		target_b_ = create_render_target(width, height);
		return target_a_ && target_b_;
	}

	void Blur::apply(Bitmap *source, int x, int y, int width, int height) const
	{
		const bool flipVertical = graphics_backend() == GraphicsBackend::opengl;
		if (!source || !is_valid() || !upload_bitmap(source))
			return;
		if (width <= 0)
			width = screen_width();
		if (height <= 0)
			height = screen_height();
		if (width <= 0 || height <= 0 || !ensure_targets(width, height))
			return;
		Bitmap *input = source;
		Bitmap *horizontal = target_a_;
		Bitmap *vertical = target_b_;
		float projection[16];
		for (int iteration = 0; iteration < iterations_; ++iteration)
		{
			begin_render_target(horizontal);
			clear_render_target({0, 0, 0, 0});
			shader_.set_uniform("source", 0);
			shader_.set_uniform("premultipliedSource", 1);
			shader_.set_uniform("premultipliedOutput", 1);
			shader_.set_uniform("opacity", 1.0f);
			shader_.set_uniform("texel", 1.0f / width, 1.0f / height);
			shader_.set_uniform("radius", radius_);
			shader_.set_uniform("direction", 1.0f, 0.0f);
			detail::gl2d_ortho_matrix(width, height, projection);
			shader_.set_uniform_mat4("uProjection", projection);
			submit_fullscreen_quad(0, 0, width, height, input->gpu_texture, iteration == 0 ? flipVertical : false);
			end_render_target();
			input = horizontal;
			begin_render_target(vertical);
			clear_render_target({0, 0, 0, 0});
			shader_.set_uniform("direction", 0.0f, 1.0f);
			submit_fullscreen_quad(0, 0, width, height, input->gpu_texture, false);
			end_render_target();
			input = vertical;
		}
		shader_.set_uniform("source", 0);
		shader_.set_uniform("premultipliedOutput", 1);
		shader_.set_uniform("opacity", opacity_);
		detail::gl2d_ortho_matrix(screen_width(), screen_height(), projection);
		shader_.set_uniform_mat4("uProjection", projection);
		submit_fullscreen_quad(x, y, width, height, input->gpu_texture, false);
		Shader::stop();
	}

} // namespace sl
