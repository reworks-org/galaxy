/** @file
 * @brief Implements the Bloom post-process effect.
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

	Bloom::~Bloom()
	{
		shutdown();
	}

	Bloom::Bloom(Bloom &&other) noexcept
		: brightShader_(std::move(other.brightShader_)),
		  blurShader_(std::move(other.blurShader_)),
		  compositeShader_(std::move(other.compositeShader_)),
		  threshold_(other.threshold_),
		  intensity_(other.intensity_),
		  radius_(other.radius_),
		  downsample_(other.downsample_),
		  blurTargetA_(std::exchange(other.blurTargetA_, nullptr)),
		  blurTargetB_(std::exchange(other.blurTargetB_, nullptr))
	{
	}

	Bloom &Bloom::operator=(Bloom &&other) noexcept
	{
		if (this != &other)
		{
			shutdown();
			brightShader_ = std::move(other.brightShader_);
			blurShader_ = std::move(other.blurShader_);
			compositeShader_ = std::move(other.compositeShader_);
			threshold_ = other.threshold_;
			intensity_ = other.intensity_;
			radius_ = other.radius_;
			downsample_ = other.downsample_;
			blurTargetA_ = std::exchange(other.blurTargetA_, nullptr);
			blurTargetB_ = std::exchange(other.blurTargetB_, nullptr);
		}
		return *this;
	}

	bool Bloom::initialise()
	{
		if (is_valid())
		{
			return true;
		}
		const std::string vertex = load_glsl_shader("fullscreen.vert");
		return !vertex.empty() &&
			   brightShader_.load(vertex, load_glsl_shader("bright_pass.frag"), "bloom-bright") &&
			   blurShader_.load(vertex, load_glsl_shader("blur.frag"), "bloom-blur") &&
			   compositeShader_.load(vertex, load_glsl_shader("composite.frag"), "bloom-composite");
	}

	void Bloom::shutdown()
	{
		brightShader_.reset();
		blurShader_.reset();
		compositeShader_.reset();
		destroy_bitmap(blurTargetA_);
		destroy_bitmap(blurTargetB_);
		blurTargetA_ = nullptr;
		blurTargetB_ = nullptr;
	}

	bool Bloom::is_valid() const
	{
		return brightShader_.is_valid() && blurShader_.is_valid() && compositeShader_.is_valid();
	}

	const std::string &Bloom::error() const
	{
		if (!brightShader_.is_valid())
			return brightShader_.error();
		if (!blurShader_.is_valid())
			return blurShader_.error();
		return compositeShader_.error();
	}

	void Bloom::set_threshold(float threshold)
	{
		threshold_ = std::clamp(threshold, 0.0f, 1.0f);
	}

	void Bloom::set_intensity(float intensity)
	{
		intensity_ = std::max(intensity, 0.0f);
	}

	void Bloom::set_radius(float radius)
	{
		radius_ = std::max(radius, 0.0f);
	}

	void Bloom::set_downsample(int factor)
	{
		downsample_ = std::max(1, factor);
	}

	bool Bloom::ensure_targets(int width, int height) const
	{
		if (blurTargetA_ && blurTargetB_ && blurTargetA_->width == width && blurTargetA_->height == height)
		{
			return true;
		}
		destroy_bitmap(blurTargetA_);
		destroy_bitmap(blurTargetB_);
		blurTargetA_ = create_render_target(width, height);
		blurTargetB_ = create_render_target(width, height);
		return blurTargetA_ != nullptr && blurTargetB_ != nullptr;
	}

	void Bloom::apply(Bitmap *source, int x, int y, int width, int height) const
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

		const int smallWidth = std::max(1, width / downsample_);
		const int smallHeight = std::max(1, height / downsample_);
		const bool opengl = graphics_backend() == GraphicsBackend::opengl;
		const float effectiveIntensity = opengl ? intensity_ * 0.12f : intensity_;
		const float effectiveRadius = opengl ? radius_ * 0.45f : radius_;
		if (!ensure_targets(smallWidth, smallHeight))
		{
			return;
		}

		float projection[16];

		// pass 1: threshold + downsample source into blurTargetA_
		begin_render_target(blurTargetA_);
		clear_render_target(Colour{0, 0, 0, 0});
		brightShader_.set_uniform("source", 0);
		brightShader_.set_uniform("threshold", threshold_);
		detail::gl2d_ortho_matrix(smallWidth, smallHeight, projection);
		brightShader_.set_uniform_mat4("uProjection", projection);
		submit_fullscreen_quad(0, 0, smallWidth, smallHeight, source->gpu_texture, flipVertical, false);
		end_render_target();

		// repeat the separable blur several times: a small single pass can only spread a
		// handful of texels, so iterating approximates a much wider Gaussian on the cheap
		Bitmap *blurSource = blurTargetA_;
		Bitmap *blurDestination = blurTargetB_;
		constexpr int blurIterations = 4;
		for (int iteration = 0; iteration < blurIterations; ++iteration)
		{
			// horizontal
			begin_render_target(blurDestination);
			clear_render_target(Colour{0, 0, 0, 0});
			blurShader_.set_uniform("source", 0);
			blurShader_.set_uniform("premultipliedSource", 1);
			blurShader_.set_uniform("premultipliedOutput", 1);
			blurShader_.set_uniform("opacity", 1.0f);
			blurShader_.set_uniform("texel", 1.0f / smallWidth, 1.0f / smallHeight);
			blurShader_.set_uniform("radius", effectiveRadius);
			blurShader_.set_uniform("direction", 1.0f, 0.0f);
			detail::gl2d_ortho_matrix(smallWidth, smallHeight, projection);
			blurShader_.set_uniform_mat4("uProjection", projection);
			submit_fullscreen_quad(0, 0, smallWidth, smallHeight, blurSource->gpu_texture, false);
			end_render_target();
			std::swap(blurSource, blurDestination);

			// vertical
			begin_render_target(blurDestination);
			clear_render_target(Colour{0, 0, 0, 0});
			blurShader_.set_uniform("direction", 0.0f, 1.0f);
			submit_fullscreen_quad(0, 0, smallWidth, smallHeight, blurSource->gpu_texture, false);
			end_render_target();
			std::swap(blurSource, blurDestination);
		}

		// pass 4: composite the blurred glow back over the full-resolution source
		compositeShader_.set_uniform("intensity", effectiveIntensity);
		if (detail::Renderer *renderer = detail::active_renderer())
			renderer->bind_texture_unit(1, blurSource->gpu_texture);
		compositeShader_.set_uniform("bloomTex", 1);
		compositeShader_.set_uniform("source", 0);
		detail::gl2d_ortho_matrix(screen_width(), screen_height(), projection);
		compositeShader_.set_uniform_mat4("uProjection", projection);
		submit_fullscreen_quad(x, y, width, height, source->gpu_texture, flipVertical);
		Shader::stop();
	}

} // namespace sl
