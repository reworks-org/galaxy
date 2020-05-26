///
/// Texture.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include <glad/glad.h>
#include <stb_image.h>
#include <pulsar/Log.hpp>
#include <stb_image_write.h>

#include "qs/core/WindowSettings.hpp"

#include "Texture.hpp"

///
/// Core namespace.
///
namespace qs
{
	Texture::Texture() noexcept
		:BaseTexture()
	{
	}

	Texture::~Texture() noexcept
	{
	}

	void Texture::load(const std::string& file) noexcept
	{
		// Generate texture in OpenGL and bind to 2D texture.
		glBindTexture(GL_TEXTURE_2D, m_texture);

		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(file.c_str(), &m_width, &m_height, nullptr, STBI_rgb_alpha);

		if (data)
		{
			// Gen texture into OpenGL.
			glTexImage2D(GL_TEXTURE_2D, 0, qs::WindowSettings::s_textureFormat, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

			// Ansiotropic filtering.
			setAnisotropy(qs::WindowSettings::s_ansiotropicFiltering);

			// Linear filtering for non-pixel as default.
			setMinifyFilter(qs::TextureFilter::LINEAR);
			setMagnifyFilter(qs::TextureFilter::LINEAR);

			// Default clamp to edge.
			clampToEdge();
		}
		else
		{
			std::string msg = "Failed to load texture: " + file + " Reason: " + stbi_failure_reason();
			PL_LOG(PL_ERROR, msg);
		}

		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::load(const unsigned char* mem, const unsigned int size) noexcept
	{
		// Generate texture in OpenGL and bind to 2D texture.
		glBindTexture(GL_TEXTURE_2D, m_texture);

		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load_from_memory(mem, size, &m_width, &m_height, nullptr, STBI_rgb_alpha);

		if (data)
		{
			// Gen texture into OpenGL.
			glTexImage2D(GL_TEXTURE_2D, 0, qs::WindowSettings::s_textureFormat, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

			// Ansiotropic filtering.
			setAnisotropy(qs::WindowSettings::s_ansiotropicFiltering);

			// Set filtering. When minimizing texture, linear interpolate, else nearest for nice pixel 2d art look.
			setMinifyFilter(qs::TextureFilter::LINEAR);

			// Set interpolation for mipmapping.
			setMagnifyFilter(qs::TextureFilter::LINEAR);

			// Default clamp to edge.
			clampToEdge();
		}
		else
		{
			std::string msg = "Failed to load texture with size: " + std::to_string(size) + " Reason: " + stbi_failure_reason();
			PL_LOG(PL_ERROR, msg);
		}

		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::load(const unsigned int id, const int width, const int height) noexcept
	{
		m_texture = id;
		m_width = width;
		m_height = height;

		// Ansiotropic filtering.
		setAnisotropy(qs::WindowSettings::s_ansiotropicFiltering);

		// Set filtering. When minimizing texture, linear interpolate, else nearest for nice pixel 2d art look.
		setMinifyFilter(qs::TextureFilter::LINEAR);

		// Set interpolation for mipmapping.
		setMagnifyFilter(qs::TextureFilter::LINEAR);

		// Default clamp to edge.
		clampToEdge();
	}

	void Texture::load(int level, int internalformat, int width, int height, int border, unsigned int format, unsigned int type, const void* pixels)
	{
		m_width = width;
		m_height = height;

		// Generate texture in OpenGL and bind to 2D texture.
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		// Gen texture into OpenGL.
		glTexImage2D(GL_TEXTURE_2D, level, internalformat, width, height, border, format, type, pixels);

		// Ansiotropic filtering.
		setAnisotropy(qs::WindowSettings::s_ansiotropicFiltering);

		// Linear filtering for non-pixel as default.
		setMinifyFilter(qs::TextureFilter::LINEAR);
		setMagnifyFilter(qs::TextureFilter::LINEAR);

		// Default clamp to edge.
		clampToEdge();
	}

	void Texture::bind() noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}

	void Texture::unbind() noexcept
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}