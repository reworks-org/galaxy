///
/// Texture.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include <glad/glad.h>
#include <pulsar/Log.hpp>
#include <stb_image.h>
#include <stb_image_write.h>

#include "qs/core/WindowSettings.hpp"

#include "Texture.hpp"

///
/// Core namespace.
///
namespace qs
{
	Texture::Texture()
	    : BaseTexture {}
	{
	}

	Texture::~Texture() noexcept
	{
	}

	void Texture::load(std::string_view file)
	{
		// Generate texture in OpenGL and bind to 2D texture.
		glBindTexture(GL_TEXTURE_2D, m_texture);

		stbi_set_flip_vertically_on_load(true);

		auto dest           = std::filesystem::path {file};
		unsigned char* data = stbi_load(dest.string().c_str(), &m_width, &m_height, nullptr, STBI_rgb_alpha);

		if (data)
		{
			// Gen texture into OpenGL.
			glTexImage2D(GL_TEXTURE_2D, 0, qs::WindowSettings::s_texture_format, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

			// Ansiotropic filtering.
			set_anisotropy(qs::WindowSettings::s_ansio_filtering);

			// Linear filtering for non-pixel as default.
			set_minify_filter(qs::TextureFilter::LINEAR);
			set_magnify_filter(qs::TextureFilter::LINEAR);

			// Default clamp to edge.
			clamp_to_edge();
		}
		else
		{
			PL_LOG(PL_ERROR, "Failed to load texture: {0}. {1}.", file, stbi_failure_reason());
		}

		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::load(const std::span<unsigned char> buffer)
	{
		// Generate texture in OpenGL and bind to 2D texture.
		glBindTexture(GL_TEXTURE_2D, m_texture);

		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load_from_memory(buffer.data(), buffer.size_bytes(), &m_width, &m_height, nullptr, STBI_rgb_alpha);

		if (data)
		{
			// Gen texture into OpenGL.
			glTexImage2D(GL_TEXTURE_2D, 0, qs::WindowSettings::s_texture_format, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

			// Ansiotropic filtering.
			set_anisotropy(qs::WindowSettings::s_ansio_filtering);

			// Set filtering. When minimizing texture, linear interpolate, else nearest for nice pixel 2d art look.
			set_minify_filter(qs::TextureFilter::LINEAR);

			// Set interpolation for mipmapping.
			set_magnify_filter(qs::TextureFilter::LINEAR);

			// Default clamp to edge.
			clamp_to_edge();
		}
		else
		{
			PL_LOG(PL_ERROR, "Failed to load texture from memory: {0}.", stbi_failure_reason());
		}

		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::load(const pr::positive_uint auto id, const pr::positive_int auto width, const pr::positive_int auto height)
	{
		m_texture = id;
		m_width   = width;
		m_height  = height;

		// Ansiotropic filtering.
		set_anisotropy(qs::WindowSettings::s_ansio_filtering);

		// Set filtering. When minimizing texture, linear interpolate, else nearest for nice pixel 2d art look.
		set_minify_filter(qs::TextureFilter::LINEAR);

		// Set interpolation for mipmapping.
		set_magnify_filter(qs::TextureFilter::LINEAR);

		// Default clamp to edge.
		clamp_to_edge();
	}

	void Texture::load(pr::positive_int auto level, pr::positive_int auto internalformat, pr::positive_int auto width, pr::positive_int auto height, pr::positive_int auto border, pr::positive_uint auto format, pr::positive_uint auto type, const pr::not_nullptr_void auto pixels)
	{
		m_width  = width;
		m_height = height;

		// Generate texture in OpenGL and bind to 2D texture.
		glBindTexture(GL_TEXTURE_2D, m_texture);

		// Gen texture into OpenGL.
		glTexImage2D(GL_TEXTURE_2D, level, internalformat, width, height, border, format, type, pixels);

		// Ansiotropic filtering.
		set_anisotropy(qs::WindowSettings::s_ansio_filtering);

		// Set filtering. When minimizing texture, linear interpolate, else nearest for nice pixel 2d art look.
		set_minify_filter(qs::TextureFilter::LINEAR);

		// Set interpolation for mipmapping.
		set_magnify_filter(qs::TextureFilter::LINEAR);

		// Default clamp to edge.
		clamp_to_edge();

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::bind() noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}

	void Texture::unbind() noexcept
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
} // namespace qs