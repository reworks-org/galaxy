/** @file
 * @brief Implements the tiled LightingPass effect (2D lights, shadows, and shadow casters).
 */
#include "graphics_fx.h"
#include "graphics_fx_internal.h"

#include "display.h"
#include "draw.h"
#include "gl2d.h"
#include "renderer.h"

#include <algorithm>
#include <cmath>
#include <utility>

namespace sl
{
	using detail::load_glsl_shader;
	using detail::submit_fullscreen_quad;

	namespace
	{
		struct GpuLight
		{
			float positionRadius[4];
			float colourIntensity[4];
			float shadowSoftness[4];
		};

		/** Append the light-facing edges of casters that can reach the light. Returns the number appended. */
		std::uint32_t append_light_edges(std::vector<float> &edges, const Light &light,
										 const std::vector<ShadowCaster> &casters)
		{
			std::uint32_t count = 0;
			for (const ShadowCaster &caster : casters)
			{
				if (caster.vertices.size() < 2)
				{
					continue;
				}
				// A caster wholly outside the light's radius cannot shadow anything the light reaches.
				if (light.radius > 0.0f)
				{
					float minX = caster.vertices[0].x, maxX = minX, minY = caster.vertices[0].y, maxY = minY;
					for (const ShadowPoint &vertex : caster.vertices)
					{
						minX = std::min(minX, vertex.x);
						maxX = std::max(maxX, vertex.x);
						minY = std::min(minY, vertex.y);
						maxY = std::max(maxY, vertex.y);
					}
					// Margin covers the soft-shadow sampling offsets.
					const float reach = light.radius + 4.0f * std::max(light.shadow_softness, 0.0f);
					const float dx = std::clamp(light.x, minX, maxX) - light.x;
					const float dy = std::clamp(light.y, minY, maxY) - light.y;
					if (dx * dx + dy * dy > reach * reach)
					{
						continue;
					}
				}
				for (std::size_t index = 0; index < caster.vertices.size(); ++index)
				{
					const ShadowPoint &first = caster.vertices[index];
					const ShadowPoint &second = caster.vertices[(index + 1) % caster.vertices.size()];
					const float normalX = second.y - first.y;
					const float normalY = -(second.x - first.x);
					const float facing = (light.x - (first.x + second.x) * 0.5f) * normalX +
										 (light.y - (first.y + second.y) * 0.5f) * normalY;
					if (facing <= 0.0f)
					{
						continue;
					}
					edges.insert(edges.end(), {first.x, first.y, second.x, second.y});
					++count;
				}
			}
			return count;
		}

	} // namespace

	ShadowCaster make_rectangle_shadow_caster(float left, float top, float right, float bottom)
	{
		return ShadowCaster{{
			{left, top},
			{right, top},
			{right, bottom},
			{left, bottom},
		}};
	}

	LightingPass::~LightingPass()
	{
		shutdown();
	}

	LightingPass::LightingPass(LightingPass &&other) noexcept
		: shader_(std::move(other.shader_)), cullShader_(std::move(other.cullShader_)), ambient_(other.ambient_),
		  lightBuffer_(std::exchange(other.lightBuffer_, 0)),
		  tileCountsBuffer_(std::exchange(other.tileCountsBuffer_, 0)),
		  tileIndicesBuffer_(std::exchange(other.tileIndicesBuffer_, 0)),
		  shadowEdgesBuffer_(std::exchange(other.shadowEdgesBuffer_, 0)),
		  tileCountX_(other.tileCountX_), tileCountY_(other.tileCountY_)
	{
	}

	LightingPass &LightingPass::operator=(LightingPass &&other) noexcept
	{
		if (this != &other)
		{
			shutdown();
			shader_ = std::move(other.shader_);
			cullShader_ = std::move(other.cullShader_);
			ambient_ = other.ambient_;
			shadowEdgesBuffer_ = std::exchange(other.shadowEdgesBuffer_, 0);
			lightBuffer_ = std::exchange(other.lightBuffer_, 0);
			tileCountsBuffer_ = std::exchange(other.tileCountsBuffer_, 0);
			tileIndicesBuffer_ = std::exchange(other.tileIndicesBuffer_, 0);
			tileCountX_ = other.tileCountX_;
			tileCountY_ = other.tileCountY_;
		}
		return *this;
	}

	bool LightingPass::initialise()
	{
		if (is_valid())
		{
			return true;
		}
		if (!shader_.load(load_glsl_shader("fullscreen.vert"), load_glsl_shader("lighting.frag"), "lighting") ||
			!cullShader_.load_compute(load_glsl_shader("light_cull.comp"), "light-cull"))
		{
			return false;
		}
		if (!detail::active_renderer()->create_storage_buffer(sizeof(GpuLight), lightBuffer_) ||
			!detail::active_renderer()->create_storage_buffer(sizeof(std::uint32_t), tileCountsBuffer_) ||
			!detail::active_renderer()->create_storage_buffer(sizeof(std::uint32_t), tileIndicesBuffer_) ||
			!detail::active_renderer()->create_storage_buffer(sizeof(float) * 4, shadowEdgesBuffer_))
			return false;
		return true;
	}

	void LightingPass::shutdown()
	{
		shader_.reset();
		if (lightBuffer_ != 0)
		{
			detail::active_renderer()->destroy_storage_buffer(lightBuffer_);
			lightBuffer_ = 0;
		}
		if (tileCountsBuffer_ != 0)
		{
			detail::active_renderer()->destroy_storage_buffer(tileCountsBuffer_);
			tileCountsBuffer_ = 0;
		}
		if (tileIndicesBuffer_ != 0)
		{
			detail::active_renderer()->destroy_storage_buffer(tileIndicesBuffer_);
			tileIndicesBuffer_ = 0;
		}
		if (shadowEdgesBuffer_ != 0)
		{
			detail::active_renderer()->destroy_storage_buffer(shadowEdgesBuffer_);
			shadowEdgesBuffer_ = 0;
		}
	}

	bool LightingPass::is_valid() const
	{
		return shader_.is_valid() && cullShader_.is_valid() && lightBuffer_ != 0 &&
			   tileCountsBuffer_ != 0 && tileIndicesBuffer_ != 0 && shadowEdgesBuffer_ != 0;
	}

	const std::string &LightingPass::error() const
	{
		return shader_.error();
	}

	void LightingPass::set_ambient(float ambient)
	{
		ambient_ = std::clamp(ambient, 0.0f, 1.0f);
	}

	void LightingPass::apply(Bitmap *source, const Light &light, int x, int y, int width, int height,
							 const std::vector<ShadowCaster> &casters) const
	{
		apply(source, std::vector<Light>{light}, x, y, width, height, casters);
	}

	void LightingPass::apply(Bitmap *source, const std::vector<Light> &lights, int x, int y, int width, int height,
							 const std::vector<ShadowCaster> &casters) const
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
		const bool sourceFlipVertical = graphics_backend() == GraphicsBackend::vulkan ? false : flipVertical;
		const std::size_t lightCount = lights.size();
		const std::size_t shadowLightCount = casters.empty()
												 ? 0
												 : std::min<std::size_t>(lightCount, max_shadow_lights);
		// Shadow edges are evaluated per pixel in the lighting shader. The buffer starts with one
		// (first edge, edge count) header per shadow light, followed by edges as (x1, y1, x2, y2).
		std::vector<float> shadowData(shadowLightCount * 4, 0.0f);
		std::uint32_t edgeOffset = static_cast<std::uint32_t>(shadowLightCount);
		for (std::size_t index = 0; index < shadowLightCount; ++index)
		{
			const std::uint32_t edgeCount = append_light_edges(shadowData, lights[index], casters);
			shadowData[index * 4] = static_cast<float>(edgeOffset);
			shadowData[index * 4 + 1] = static_cast<float>(edgeCount);
			edgeOffset += edgeCount;
		}
		if (shadowData.empty())
		{
			shadowData.assign(4, 0.0f);
		}

		std::vector<GpuLight> gpuLights(lightCount);
		for (std::size_t index = 0; index < lightCount; ++index)
		{
			const Light &light = lights[index];
			GpuLight &gpuLight = gpuLights[index];
			gpuLight.positionRadius[0] = light.x;
			gpuLight.positionRadius[1] = light.y;
			gpuLight.positionRadius[2] = std::max(light.radius, 0.0f);
			const float directionLength = std::sqrt(light.direction_x * light.direction_x + light.direction_y * light.direction_y);
			const bool spotlight = light.inner_angle > 0.0f && light.outer_angle > 0.0f && directionLength > 0.0001f;
			gpuLight.positionRadius[3] = 0.0f;
			gpuLight.colourIntensity[0] = static_cast<float>(light.colour.red) / 255.0f;
			gpuLight.colourIntensity[1] = static_cast<float>(light.colour.green) / 255.0f;
			gpuLight.colourIntensity[2] = static_cast<float>(light.colour.blue) / 255.0f;
			gpuLight.colourIntensity[3] = std::max(light.intensity, 0.0f);
			gpuLight.shadowSoftness[0] = std::max(light.shadow_softness, 0.0f);
			gpuLight.shadowSoftness[1] = spotlight ? light.direction_x / directionLength : 0.0f;
			gpuLight.shadowSoftness[2] = spotlight ? light.direction_y / directionLength : 0.0f;
			gpuLight.shadowSoftness[3] = spotlight
											 ? std::cos(std::clamp(light.outer_angle, 0.0f, 179.0f) * 3.14159265358979323846f / 180.0f)
											 : -1.0f;
			gpuLight.positionRadius[3] = spotlight
											 ? std::cos(std::clamp(light.inner_angle, 0.0f, 179.0f) * 3.14159265358979323846f / 180.0f)
											 : 0.0f;
		}
		detail::Renderer *renderer = detail::active_renderer();
		renderer->upload_storage_buffer(lightBuffer_, gpuLights.size() * sizeof(GpuLight), gpuLights.data(), false);
		renderer->bind_storage_buffer(2, lightBuffer_);
		renderer->upload_storage_buffer(shadowEdgesBuffer_, shadowData.size() * sizeof(float), shadowData.data(), false);
		renderer->bind_storage_buffer(5, shadowEdgesBuffer_);

		const int tileCountX = (source->width + tile_size - 1) / tile_size;
		const int tileCountY = (source->height + tile_size - 1) / tile_size;
		const bool tileBuffersNeedResize = tileCountX != tileCountX_ || tileCountY != tileCountY_;
		tileCountX_ = tileCountX;
		tileCountY_ = tileCountY;
		const std::size_t tileCount = static_cast<std::size_t>(tileCountX_) * tileCountY_;
		if (tileBuffersNeedResize)
		{
			renderer->upload_storage_buffer(tileCountsBuffer_, tileCount * sizeof(std::uint32_t), nullptr, false);
		}
		renderer->bind_storage_buffer(3, tileCountsBuffer_);
		if (tileBuffersNeedResize)
		{
			renderer->upload_storage_buffer(tileIndicesBuffer_, tileCount * max_lights_per_tile * sizeof(std::uint32_t), nullptr, false);
		}
		renderer->bind_storage_buffer(4, tileIndicesBuffer_);
		cullShader_.set_uniform("screenSize", source->width, source->height);
		cullShader_.set_uniform("tileCount", tileCountX_, tileCountY_);
		cullShader_.set_uniform("lightCount", static_cast<int>(lightCount));
		cullShader_.dispatch_compute(static_cast<unsigned int>(tileCountX_), static_cast<unsigned int>(tileCountY_), 1);
		renderer->storage_barrier();

		float projection[16];
		shader_.set_uniform("source", 0);
		shader_.set_uniform("lightCount", static_cast<int>(lightCount));
		shader_.set_uniform("shadowLightCount", static_cast<int>(shadowLightCount));
		shader_.set_uniform("tileCount", tileCountX_, tileCountY_);
		shader_.set_uniform("ambient", ambient_);
		shader_.set_uniform("flipVertical", sourceFlipVertical ? 1 : 0);
		detail::gl2d_ortho_matrix(screen_width(), screen_height(), projection);
		shader_.set_uniform_mat4("uProjection", projection);
		submit_fullscreen_quad(x, y, width, height, source->gpu_texture, sourceFlipVertical);
		Shader::stop();
	}

} // namespace sl
