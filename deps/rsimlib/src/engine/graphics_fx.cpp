/** @file
 * @brief Common graphics_fx infrastructure shared by the individual effect implementations:
 * the ping-pong buffer, the Shader wrapper, shared helpers, and screen fades. Each effect
 * (Bloom, Vignette, CRTFilter, etc.) is implemented in its own graphics_fx_*.cpp file.
 */

#define GL_GLEXT_PROTOTYPES

#include "graphics_fx.h"
#include "graphics_fx_internal.h"

#include "display.h"
#include "draw.h"
#include "gl2d.h"
#include "renderer.h"

#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>
#include <SDL2/SDL.h>

#include <filesystem>
#include <fstream>
#include <iterator>
#include <utility>

namespace sl
{
	PingPongBuffer::~PingPongBuffer()
	{
		shutdown();
	}

	void PingPongBuffer::initialise(int width, int height)
	{
		shutdown();
		width_ = width;
		height_ = height;
		buffers_[0] = create_render_target(width, height);
		buffers_[1] = create_render_target(width, height);
		if (!buffers_[0] || !buffers_[1])
		{
			shutdown();
			return;
		}
		next_index_ = 0;
		source_ = nullptr;
		current_target_ = buffers_[0];
	}

	void PingPongBuffer::shutdown()
	{
		for (Bitmap *buffer : buffers_)
		{
			if (buffer)
			{
				destroy_bitmap(buffer);
			}
		}
		buffers_[0] = nullptr;
		buffers_[1] = nullptr;
		source_ = nullptr;
		current_target_ = nullptr;
		width_ = 0;
		height_ = 0;
		next_index_ = 0;
	}

	bool PingPongBuffer::valid() const
	{
		return buffers_[0] != nullptr && buffers_[1] != nullptr && width_ > 0 && height_ > 0;
	}

	Bitmap *PingPongBuffer::begin(Bitmap *source)
	{
		if (!valid())
			return nullptr;
		source_ = source;
		current_target_ = buffers_[next_index_];
		next_index_ = 1 - next_index_;
		return current_target_;
	}

	Bitmap *PingPongBuffer::source() const
	{
		return source_;
	}

	Bitmap *PingPongBuffer::target() const
	{
		return current_target_;
	}

	Bitmap *PingPongBuffer::advance()
	{
		if (!valid() || !current_target_)
			return nullptr;
		source_ = current_target_;
		return source_;
	}

	namespace detail
	{
		std::string load_glsl_shader(const char *name)
		{
			std::ifstream file(std::filesystem::path(SIMLIB_GLSL_SHADER_DIR) / name);
			if (!file)
			{
				file.clear();
				file.open(std::filesystem::path(SIMLIB_BUILD_GLSL_SHADER_DIR) / name);
			}
			return file ? std::string(std::istreambuf_iterator<char>(file), {}) : std::string{};
		}

		void submit_fullscreen_quad(int x, int y, int width, int height, GLuint texture,
									bool flipVertical, bool premultipliedAlpha)
		{
			const float left = static_cast<float>(x);
			const float top = static_cast<float>(y);
			const float right = static_cast<float>(x + width);
			const float bottom = static_cast<float>(y + height);
			const float topV = flipVertical ? 1.0f : 0.0f;
			const float bottomV = flipVertical ? 0.0f : 1.0f;
			const detail::GLVertex vertices[4] = {
				{left, top, 0.0f, topV, 1.0f, 1.0f, 1.0f, 1.0f},
				{right, top, 1.0f, topV, 1.0f, 1.0f, 1.0f, 1.0f},
				{right, bottom, 1.0f, bottomV, 1.0f, 1.0f, 1.0f, 1.0f},
				{left, bottom, 0.0f, bottomV, 1.0f, 1.0f, 1.0f, 1.0f},
			};
			if (detail::Renderer *renderer = detail::active_renderer())
				renderer->set_premultiplied_alpha(premultipliedAlpha);
			detail::gl2d_submit(GL_TRIANGLE_FAN, vertices, 4, texture);
		}
	} // namespace detail

	Shader::Shader(const std::string &vertexSource, const std::string &fragmentSource)
	{
		load(vertexSource, fragmentSource);
	}

	Shader::~Shader()
	{
		reset();
	}

	Shader::Shader(Shader &&other) noexcept
		: program_(std::exchange(other.program_, 0)), error_(std::move(other.error_))
	{
	}

	Shader &Shader::operator=(Shader &&other) noexcept
	{
		if (this != &other)
		{
			reset();
			program_ = std::exchange(other.program_, 0);
			error_ = std::move(other.error_);
		}
		return *this;
	}

	bool Shader::load(const std::string &vertexSource, const std::string &fragmentSource)
	{
		return load(vertexSource, fragmentSource, {});
	}

	bool Shader::load(const std::string &vertexSource, const std::string &fragmentSource, const std::string &assetId)
	{
		reset();
		detail::Renderer *renderer = detail::active_renderer();
		if (!renderer)
		{
			error_ = "No active renderer is available for shader creation.";
			return false;
		}
		return renderer->create_shader(
			detail::ShaderSource{detail::ShaderLanguage::glsl, vertexSource, {}, assetId},
			detail::ShaderSource{detail::ShaderLanguage::glsl, fragmentSource, {}, assetId},
			program_, error_);
	}

	bool Shader::load(const std::string &vertexSource, const std::string &fragmentSource,
					  const std::vector<std::string> &vulkanSamplerNames,
					  const std::vector<ShaderUniform> &vulkanUniforms)
	{
		reset();
		detail::Renderer *renderer = detail::active_renderer();
		if (!renderer)
		{
			error_ = "No active renderer is available for shader creation.";
			return false;
		}
		std::vector<detail::ShaderUniformLayout> uniforms;
		uniforms.reserve(vulkanUniforms.size());
		for (const ShaderUniform &uniform : vulkanUniforms)
			uniforms.push_back({uniform.name, uniform.offset, uniform.size});
		detail::ShaderSource vertex{detail::ShaderLanguage::glsl, vertexSource, {}, {}};
		detail::ShaderSource fragment{detail::ShaderLanguage::glsl, fragmentSource, {}, {}};
		fragment.vulkan_sampler_names = vulkanSamplerNames;
		fragment.vulkan_uniforms = uniforms;
		return renderer->create_shader(vertex, fragment, program_, error_);
	}

	bool Shader::load_files(const std::string &vertexPath, const std::string &fragmentPath,
							const std::vector<std::string> &vulkanSamplerNames,
							const std::vector<ShaderUniform> &vulkanUniforms, const std::string &assetId)
	{
		reset();
		std::ifstream vertexFile(vertexPath, std::ios::binary);
		std::ifstream fragmentFile(fragmentPath, std::ios::binary);
		if (!vertexFile || !fragmentFile)
		{
			error_ = !vertexFile ? "Unable to open vertex shader file: " + vertexPath
								 : "Unable to open fragment shader file: " + fragmentPath;
			return false;
		}
		const std::string vertexSource(std::istreambuf_iterator<char>(vertexFile), {});
		const std::string fragmentSource(std::istreambuf_iterator<char>(fragmentFile), {});
		if (vertexSource.empty() || fragmentSource.empty())
		{
			error_ = vertexSource.empty() ? "Vertex shader file is empty: " + vertexPath
										  : "Fragment shader file is empty: " + fragmentPath;
			return false;
		}
		if (vulkanSamplerNames.empty() && vulkanUniforms.empty() && assetId.empty())
			return load(vertexSource, fragmentSource);
		detail::Renderer *renderer = detail::active_renderer();
		if (!renderer)
		{
			error_ = "No active renderer is available for shader creation.";
			return false;
		}
		std::vector<detail::ShaderUniformLayout> uniforms;
		uniforms.reserve(vulkanUniforms.size());
		for (const ShaderUniform &uniform : vulkanUniforms)
			uniforms.push_back({uniform.name, uniform.offset, uniform.size});
		detail::ShaderSource vertex{detail::ShaderLanguage::glsl, vertexSource, {}, assetId};
		detail::ShaderSource fragment{detail::ShaderLanguage::glsl, fragmentSource, {}, assetId};
		fragment.vulkan_sampler_names = vulkanSamplerNames;
		fragment.vulkan_uniforms = uniforms;
		return renderer->create_shader(vertex, fragment, program_, error_);
	}

	bool Shader::load_compute(const std::string &computeSource)
	{
		return load_compute(computeSource, std::string{});
	}

	bool Shader::load_compute(const std::string &computeSource, const std::string &assetId)
	{
		return load_compute(computeSource, {}, assetId);
	}

	bool Shader::load_compute(const std::string &computeSource, const std::vector<ShaderUniform> &vulkanUniforms,
							 const std::string &assetId)
	{
		reset();
		detail::Renderer *renderer = detail::active_renderer();
		if (!renderer)
		{
			error_ = "No active renderer is available for compute shader creation.";
			return false;
		}
		std::vector<detail::ShaderUniformLayout> uniforms;
		uniforms.reserve(vulkanUniforms.size());
		for (const ShaderUniform &uniform : vulkanUniforms)
			uniforms.push_back({uniform.name, uniform.offset, uniform.size});
		detail::ShaderSource source{detail::ShaderLanguage::glsl, computeSource, {}, assetId};
		source.vulkan_uniforms = uniforms;
		return renderer->create_compute_shader(source, program_, error_);
	}

	bool Shader::load_compute_file(const std::string &computePath, const std::string &assetId)
	{
		reset();
		std::ifstream file(computePath, std::ios::binary);
		if (!file)
		{
			error_ = "Unable to open compute shader file: " + computePath;
			return false;
		}
		const std::string source(std::istreambuf_iterator<char>(file), {});
		if (source.empty())
		{
			error_ = "Compute shader file is empty: " + computePath;
			return false;
		}
		return load_compute(source, assetId);
	}

	void Shader::reset()
	{
		if (program_ != 0)
		{
			if (detail::Renderer *renderer = detail::active_renderer())
			{
				renderer->destroy_shader(program_);
			}
			program_ = 0;
		}
		error_.clear();
	}

	bool Shader::is_valid() const
	{
		return program_ != 0;
	}

	const std::string &Shader::error() const
	{
		return error_;
	}

	bool Shader::use() const
	{
		if (!is_valid())
		{
			return false;
		}
		return detail::active_renderer() && detail::active_renderer()->use_shader(program_);
	}

	bool Shader::draw_textured_quad(Bitmap *texture, float x, float y, float width, float height) const
	{
		if (!is_valid() || !texture || width <= 0.0f || height <= 0.0f || !upload_bitmap(texture))
			return false;
		detail::Renderer *renderer = detail::active_renderer();
		if (!renderer || !renderer->begin_shader_2d(program_, screen_width(), screen_height()))
			return false;
		const bool flipVertical = graphics_backend() == GraphicsBackend::opengl;
		const float top = flipVertical ? 1.0f : 0.0f;
		const float bottom = flipVertical ? 0.0f : 1.0f;
		const detail::Vertex2D vertices[4] = {
			{x, y, 0.0f, top, 1.0f, 1.0f, 1.0f, 1.0f},
			{x + width, y, 1.0f, top, 1.0f, 1.0f, 1.0f, 1.0f},
			{x + width, y + height, 1.0f, bottom, 1.0f, 1.0f, 1.0f, 1.0f},
			{x, y + height, 0.0f, bottom, 1.0f, 1.0f, 1.0f, 1.0f}};
		renderer->submit_2d(detail::PrimitiveType::triangle_fan, vertices, 4, texture->gpu_texture);
		renderer->stop_shader();
		return true;
	}

	void Shader::stop()
	{
		if (detail::Renderer *renderer = detail::active_renderer())
			renderer->stop_shader();
	}

	bool Shader::dispatch_compute(unsigned int groupsX, unsigned int groupsY, unsigned int groupsZ) const
	{
		return detail::active_renderer() && detail::active_renderer()->dispatch_compute(program_, groupsX, groupsY, groupsZ);
	}

	bool Shader::set_texture(unsigned int unit, Bitmap *bitmap) const
	{
		if (!use())
		{
			return false;
		}
		return bind_texture(unit, bitmap);
	}

	bool Shader::set_texture(const char *samplerName, unsigned int unit, Bitmap *bitmap) const
	{
		if (!set_texture(unit, bitmap))
		{
			return false;
		}
		if (samplerName && *samplerName)
		{
			return set_uniform(samplerName, static_cast<int>(unit));
		}
		return true;
	}

	bool Shader::set_storage_texture(unsigned int unit, Bitmap *bitmap) const
	{
		if (!use())
		{
			return false;
		}
		if (detail::Renderer *renderer = detail::active_renderer())
		{
			renderer->bind_storage_texture(unit, bitmap ? bitmap->gpu_texture : 0);
			return true;
		}
		return false;
	}

	bool Shader::set_storage_texture(const char *samplerName, unsigned int unit, Bitmap *bitmap) const
	{
		if (!set_storage_texture(unit, bitmap))
		{
			return false;
		}
		if (samplerName && *samplerName)
		{
			return set_uniform(samplerName, static_cast<int>(unit));
		}
		return true;
	}

	bool Shader::set_uniform(const char *name, int value) const
	{
		return detail::active_renderer() && detail::active_renderer()->set_shader_int(program_, name, value);
	}

	bool Shader::set_uniform(const char *name, float value) const
	{
		return detail::active_renderer() && detail::active_renderer()->set_shader_float(program_, name, value);
	}

	bool Shader::set_uniform(const char *name, float x, float y) const
	{
		return detail::active_renderer() && detail::active_renderer()->set_shader_float2(program_, name, x, y);
	}

	bool Shader::set_uniform(const char *name, int x, int y) const
	{
		return detail::active_renderer() && detail::active_renderer()->set_shader_int2(program_, name, x, y);
	}

	bool Shader::set_uniform(const char *name, float x, float y, float z) const
	{
		return detail::active_renderer() && detail::active_renderer()->set_shader_float3(program_, name, x, y, z);
	}

	bool Shader::set_uniform_mat4(const char *name, const float *matrix4x4) const
	{
		return detail::active_renderer() && detail::active_renderer()->set_shader_mat4(program_, name, matrix4x4);
	}

	StorageBuffer::StorageBuffer(std::size_t sizeBytes)
	{
		create(sizeBytes);
	}

	StorageBuffer::~StorageBuffer()
	{
		destroy();
	}

	StorageBuffer::StorageBuffer(StorageBuffer &&other) noexcept
		: handle_(other.handle_), sizeBytes_(other.sizeBytes_)
	{
		other.handle_ = 0;
		other.sizeBytes_ = 0;
	}

	StorageBuffer &StorageBuffer::operator=(StorageBuffer &&other) noexcept
	{
		if (this != &other)
		{
			destroy();
			handle_ = other.handle_;
			sizeBytes_ = other.sizeBytes_;
			other.handle_ = 0;
			other.sizeBytes_ = 0;
		}
		return *this;
	}

	bool StorageBuffer::create(std::size_t sizeBytes)
	{
		destroy();
		if (sizeBytes == 0)
			return false;
		if (detail::Renderer *renderer = detail::active_renderer())
		{
			if (renderer->create_storage_buffer(sizeBytes, handle_))
			{
				sizeBytes_ = sizeBytes;
				return true;
			}
		}
		return false;
	}

	void StorageBuffer::destroy()
	{
		if (handle_ != 0)
		{
			if (detail::Renderer *renderer = detail::active_renderer())
			{
				renderer->destroy_storage_buffer(handle_);
			}
			handle_ = 0;
			sizeBytes_ = 0;
		}
	}

	bool StorageBuffer::is_valid() const
	{
		return handle_ != 0;
	}

	std::size_t StorageBuffer::size_bytes() const
	{
		return sizeBytes_;
	}

	std::uint32_t StorageBuffer::handle() const
	{
		return handle_;
	}

	bool StorageBuffer::upload(const void *data, std::size_t sizeBytes, std::size_t offsetBytes)
	{
		if (!is_valid() || sizeBytes == 0)
			return false;
		if (offsetBytes + sizeBytes > sizeBytes_)
			return false;
		if (detail::Renderer *renderer = detail::active_renderer())
		{
			return renderer->upload_storage_buffer(handle_, sizeBytes, data, offsetBytes > 0);
		}
		return false;
	}

	bool StorageBuffer::readback(void *outData, std::size_t sizeBytes, std::size_t offsetBytes) const
	{
		if (!is_valid() || sizeBytes == 0 || !outData)
			return false;
		if (offsetBytes + sizeBytes > sizeBytes_)
			return false;
		if (detail::Renderer *renderer = detail::active_renderer())
		{
			return renderer->readback_storage_buffer(handle_, offsetBytes, sizeBytes, outData);
		}
		return false;
	}

	bool StorageBuffer::bind(unsigned int bindingIndex) const
	{
		if (!is_valid())
			return false;
		if (detail::Renderer *renderer = detail::active_renderer())
		{
			renderer->bind_storage_buffer(bindingIndex, handle_);
			return true;
		}
		return false;
	}

	void compute_barrier()
	{
		if (detail::Renderer *renderer = detail::active_renderer())
		{
			renderer->storage_barrier();
		}
	}

	bool dispatch_compute_for(const Shader &shader,
							  unsigned int totalItemsX, unsigned int totalItemsY, unsigned int totalItemsZ,
							  unsigned int localSizeX, unsigned int localSizeY, unsigned int localSizeZ)
	{
		if (localSizeX == 0 || localSizeY == 0 || localSizeZ == 0)
			return false;
		unsigned int groupsX = (totalItemsX + localSizeX - 1) / localSizeX;
		unsigned int groupsY = (totalItemsY + localSizeY - 1) / localSizeY;
		unsigned int groupsZ = (totalItemsZ + localSizeZ - 1) / localSizeZ;
		return shader.dispatch_compute(groupsX, groupsY, groupsZ);
	}

	void ScreenFade::set_colour(Colour colour)
	{
		colour_ = colour;
	}

	Colour ScreenFade::colour() const
	{
		return colour_;
	}

	void ScreenFade::apply() const
	{
		if (colour_.alpha == 0)
		{
			return;
		}
		rectfill(screen, 0.0f, 0.0f,
				 static_cast<float>(screen_width()), static_cast<float>(screen_height()), colour_);
	}

} // namespace sl
