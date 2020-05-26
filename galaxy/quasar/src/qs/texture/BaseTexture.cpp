///
/// BaseTexture.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include <glad/glad.h>
#include <stb_image_write.h>

#include "BaseTexture.hpp"

///
/// Core namespace.
///
namespace qs
{
	BaseTexture::~BaseTexture() noexcept
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &m_texture);
	}

	void BaseTexture::save(const std::string& path) noexcept
	{
		if (!path.empty())
		{
			std::filesystem::path fp(path + ".png");
			std::vector<unsigned int> pixels(m_width * m_height * 4, 0);

			glBindTexture(GL_TEXTURE_2D, m_texture);
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

			stbi_flip_vertically_on_write(true);
			stbi_write_png(fp.string().c_str(), m_width, m_height, 4, pixels.data(), m_width * 4);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	void BaseTexture::clampToEdge() noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void BaseTexture::clampToBorder(protostar::Colour& border) noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border.asFloats().data());
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void BaseTexture::setRepeated() noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void BaseTexture::setMirrored() noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void BaseTexture::setAnisotropy(const int level) noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, level);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void BaseTexture::setMinifyFilter(const qs::TextureFilter& filter) noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
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

	void BaseTexture::setMagnifyFilter(const qs::TextureFilter& filter) noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
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

	const int BaseTexture::getWidth() const noexcept
	{
		return m_width;
	}

	const int BaseTexture::getHeight() const noexcept
	{
		return m_height;
	}

	const unsigned int BaseTexture::getGLTexture() const noexcept
	{
		return m_texture;
	}

	BaseTexture::BaseTexture() noexcept
		:m_texture(0), m_width(0), m_height(0)
	{
		glGenTextures(1, &m_texture);
	}
}