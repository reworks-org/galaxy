///
/// Texture.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include <glad/glad.h>
#include <stb_image.h>
#include <stb_image_write.h>

#include "qs/utils/Error.hpp"
#include "qs/core/WindowSettings.hpp"

#include "Texture.hpp"

///
/// Core namespace.
///
namespace qs
{
	Texture::Texture() noexcept
		:m_id(0), m_width(0), m_height(0)
	{
	}

	Texture::Texture(const std::string& file)
		:m_id(0), m_width(0), m_height(0)
	{
		load(file);
	}

	Texture::Texture(const unsigned char* mem, const unsigned int size)
		: m_id(0), m_width(0), m_height(0)
	{
		load(mem, size);
	}

	Texture::~Texture() noexcept
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &m_id);
	}

	void Texture::load(const std::string& file)
	{
		// Generate texture in OpenGL and bind to 2D texture.
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);

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
			qs::Error::handle().callback("Texture.cpp", 75, msg);
		}

		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::load(const unsigned char* mem, const unsigned int size)
	{
		// Generate texture in OpenGL and bind to 2D texture.
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);

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
			qs::Error::handle().callback("Texture.cpp", 103, msg);
		}

		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::load(const unsigned int id, const int width, const int height) noexcept
	{
		m_id = id;
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

	void Texture::save(const std::string& path) noexcept
	{
		if (!path.empty())
		{
			std::filesystem::path fp(path + ".png");
			std::vector<unsigned int> pixels(m_width * m_height * 4, 0);

			glBindTexture(GL_TEXTURE_2D, m_id);
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

			stbi_flip_vertically_on_write(true);
			stbi_write_png(fp.string().c_str(), m_width, m_height, 4, pixels.data(), m_width * 4);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	void Texture::bind() noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

	void Texture::unbind() noexcept
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::setRepeated() noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::setMirrored() noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::clampToEdge() noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::clampToBorder(protostar::Colour& border) noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_id);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border.asFloats().data());
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::setAnisotropy(const int level) noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, level);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::setMinifyFilter(const qs::TextureFilter& filter) noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_id);
		if (filter == qs::TextureFilter::LINEAR)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		else if (filter == qs::TextureFilter::NEAREST)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::setMagnifyFilter(const qs::TextureFilter& filter) noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_id);
		if (filter == qs::TextureFilter::LINEAR)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else if (filter == qs::TextureFilter::NEAREST)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	const int Texture::getWidth() const noexcept
	{
		return m_width;
	}

	const int Texture::getHeight() const noexcept
	{
		return m_height;
	}

	const unsigned int Texture::getGLTexture() const noexcept
	{
		return m_id;
	}
}