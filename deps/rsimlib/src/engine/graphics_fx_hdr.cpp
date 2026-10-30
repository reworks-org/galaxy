/** @file
 * @brief Implements the Hdr post-process effect.
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

	namespace
	{
		const char *const vulkan_vertex_source = R"(#version 450
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(push_constant) uniform Projection { mat4 value; } projection;
layout(location = 0) out vec2 uv;
void main() { gl_Position = projection.value * vec4(aPos, 0.0, 1.0); uv = aTexCoord; }
)";

		// Shared GLSL body; the header supplies the version, samplers and uniforms per backend.
		const char *const bright_body = R"(
vec3 expand(vec3 c)
{
    float a = 1.0 - 1.0 / whitePoint;
    return c / max(1.0 - a * min(c, vec3(0.999)), 0.001);
}
void main()
{
    vec3 h = expand(texture(source, uv).rgb);
    vec3 bright = max(h - vec3(threshold), vec3(0.0));
    fragColor = vec4(clamp(bright / whitePoint, 0.0, 1.0), 1.0);
}
)";

		const char *const composite_body = R"(
vec3 expand(vec3 c)
{
    float a = 1.0 - 1.0 / whitePoint;
    return c / max(1.0 - a * min(c, vec3(0.999)), 0.001);
}
vec3 tonemap(vec3 h)
{
    if (mode == 1)
    {
        vec3 v = (h * (2.51 * h + 0.03)) / (h * (2.43 * h + 0.59) + 0.14);
        return clamp(v, 0.0, 1.0);
    }
    float a = 1.0 - 1.0 / whitePoint;
    return clamp(h / (1.0 + a * h), 0.0, 1.0);
}
void main()
{
    vec4 base = texture(source, uv);
    vec3 h = expand(base.rgb) + texture(bloomTex, uv).rgb * whitePoint * intensity;
    fragColor = vec4(tonemap(h * exposure), base.a);
}
)";

		std::string glsl_fragment(const char *uniforms, const char *body)
		{
			return std::string("#version 430 core\nuniform sampler2D source;\nuniform sampler2D bloomTex;\n") + uniforms +
				   "in vec2 uv;\nout vec4 fragColor;\n" + body;
		}

		std::string vulkan_fragment(const char *block, const char *body)
		{
			return std::string("#version 450\nlayout(set = 0, binding = 0) uniform sampler2D source;\n") + block +
				   "layout(location = 0) in vec2 uv;\nlayout(location = 0) out vec4 fragColor;\n" + body;
		}
	} // namespace

	Hdr::~Hdr()
	{
		shutdown();
	}

	Hdr::Hdr(Hdr &&other) noexcept
		: brightShader_(std::move(other.brightShader_)),
		  blurShader_(std::move(other.blurShader_)),
		  compositeShader_(std::move(other.compositeShader_)),
		  exposure_(other.exposure_),
		  whitePoint_(other.whitePoint_),
		  threshold_(other.threshold_),
		  intensity_(other.intensity_),
		  radius_(other.radius_),
		  downsample_(other.downsample_),
		  tonemap_(other.tonemap_),
		  blurTargetA_(std::exchange(other.blurTargetA_, nullptr)),
		  blurTargetB_(std::exchange(other.blurTargetB_, nullptr))
	{
	}

	Hdr &Hdr::operator=(Hdr &&other) noexcept
	{
		if (this != &other)
		{
			shutdown();
			brightShader_ = std::move(other.brightShader_);
			blurShader_ = std::move(other.blurShader_);
			compositeShader_ = std::move(other.compositeShader_);
			exposure_ = other.exposure_;
			whitePoint_ = other.whitePoint_;
			threshold_ = other.threshold_;
			intensity_ = other.intensity_;
			radius_ = other.radius_;
			downsample_ = other.downsample_;
			tonemap_ = other.tonemap_;
			blurTargetA_ = std::exchange(other.blurTargetA_, nullptr);
			blurTargetB_ = std::exchange(other.blurTargetB_, nullptr);
		}
		return *this;
	}

	bool Hdr::initialise()
	{
		if (is_valid())
		{
			return true;
		}
		const std::string glVertex = load_glsl_shader("fullscreen.vert");
		const std::string blurFragment = load_glsl_shader("blur.frag");
		if (glVertex.empty() || blurFragment.empty())
		{
			return false;
		}
		if (!blurShader_.load(glVertex, blurFragment, "bloom-blur"))
		{
			return false;
		}
		if (graphics_backend() == GraphicsBackend::vulkan)
		{
			const std::string brightSource = vulkan_fragment(
				"layout(push_constant) uniform P { layout(offset = 64) float threshold; float whitePoint; } u;\n"
				"#define threshold u.threshold\n#define whitePoint u.whitePoint\n",
				bright_body);
			const std::string compositeSource = vulkan_fragment(
				"layout(set = 0, binding = 1) uniform sampler2D bloomTex;\n"
				"layout(push_constant) uniform P { layout(offset = 64) float exposure; float whitePoint; float intensity; int mode; } u;\n"
				"#define exposure u.exposure\n#define whitePoint u.whitePoint\n#define intensity u.intensity\n#define mode u.mode\n",
				composite_body);
			return brightShader_.load(vulkan_vertex_source, brightSource, {"source"},
									  {{"threshold", 0, 4}, {"whitePoint", 4, 4}}) &&
				   compositeShader_.load(vulkan_vertex_source, compositeSource, {"source", "bloomTex"},
										 {{"exposure", 0, 4}, {"whitePoint", 4, 4}, {"intensity", 8, 4}, {"mode", 12, 4}});
		}
		return brightShader_.load(glVertex,
								  glsl_fragment("uniform float threshold;\nuniform float whitePoint;\n", bright_body)) &&
			   compositeShader_.load(glVertex,
									 glsl_fragment("uniform float exposure;\nuniform float whitePoint;\nuniform float intensity;\n"
												   "uniform int mode;\n",
												   composite_body));
	}

	void Hdr::shutdown()
	{
		brightShader_.reset();
		blurShader_.reset();
		compositeShader_.reset();
		destroy_bitmap(blurTargetA_);
		destroy_bitmap(blurTargetB_);
		blurTargetA_ = nullptr;
		blurTargetB_ = nullptr;
	}

	bool Hdr::is_valid() const
	{
		return brightShader_.is_valid() && blurShader_.is_valid() && compositeShader_.is_valid();
	}

	const std::string &Hdr::error() const
	{
		if (!brightShader_.is_valid())
			return brightShader_.error();
		if (!blurShader_.is_valid())
			return blurShader_.error();
		return compositeShader_.error();
	}

	void Hdr::set_exposure(float exposure)
	{
		exposure_ = std::max(exposure, 0.0f);
	}

	void Hdr::set_white_point(float whitePoint)
	{
		whitePoint_ = std::max(whitePoint, 1.5f);
	}

	void Hdr::set_bloom_threshold(float threshold)
	{
		threshold_ = std::max(threshold, 0.0f);
	}

	void Hdr::set_bloom_intensity(float intensity)
	{
		intensity_ = std::max(intensity, 0.0f);
	}

	void Hdr::set_bloom_radius(float radius)
	{
		radius_ = std::max(radius, 0.0f);
	}

	void Hdr::set_downsample(int factor)
	{
		downsample_ = std::max(1, factor);
	}

	void Hdr::set_tonemap(HdrTonemap tonemap)
	{
		tonemap_ = tonemap;
	}

	bool Hdr::ensure_targets(int width, int height) const
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

	void Hdr::apply(Bitmap *source, int x, int y, int width, int height) const
	{
		const bool opengl = graphics_backend() == GraphicsBackend::opengl;
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
		const float effectiveRadius = opengl ? radius_ * 0.45f : radius_;
		if (!ensure_targets(smallWidth, smallHeight))
		{
			return;
		}

		float projection[16];

		// expand to scene values, keep what exceeds the threshold, and downsample
		begin_render_target(blurTargetA_);
		clear_render_target(Colour{0, 0, 0, 0});
		brightShader_.set_uniform("source", 0);
		brightShader_.set_uniform("threshold", threshold_);
		brightShader_.set_uniform("whitePoint", whitePoint_);
		detail::gl2d_ortho_matrix(smallWidth, smallHeight, projection);
		brightShader_.set_uniform_mat4("uProjection", projection);
		brightShader_.use();
		submit_fullscreen_quad(0, 0, smallWidth, smallHeight, source->gpu_texture, opengl, false);
		end_render_target();

		Bitmap *blurSource = blurTargetA_;
		Bitmap *blurDestination = blurTargetB_;
		constexpr int blurIterations = 4;
		for (int iteration = 0; iteration < blurIterations; ++iteration)
		{
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

			begin_render_target(blurDestination);
			clear_render_target(Colour{0, 0, 0, 0});
			blurShader_.set_uniform("direction", 0.0f, 1.0f);
			submit_fullscreen_quad(0, 0, smallWidth, smallHeight, blurSource->gpu_texture, false);
			end_render_target();
			std::swap(blurSource, blurDestination);
		}

		// add the glow in scene space, then expose and tone-map at full resolution
		compositeShader_.set_uniform("exposure", exposure_);
		compositeShader_.set_uniform("whitePoint", whitePoint_);
		compositeShader_.set_uniform("intensity", intensity_);
		compositeShader_.set_uniform("mode", tonemap_ == HdrTonemap::aces ? 1 : 0);
		// custom Vulkan programs only accept texture bindings while they are the active program
		compositeShader_.use();
		if (detail::Renderer *renderer = detail::active_renderer())
			renderer->bind_texture_unit(1, blurSource->gpu_texture);
		compositeShader_.set_uniform("bloomTex", 1);
		compositeShader_.set_uniform("source", 0);
		detail::gl2d_ortho_matrix(screen_width(), screen_height(), projection);
		compositeShader_.set_uniform_mat4("uProjection", projection);
		submit_fullscreen_quad(x, y, width, height, source->gpu_texture, opengl);
		Shader::stop();
	}

} // namespace sl
