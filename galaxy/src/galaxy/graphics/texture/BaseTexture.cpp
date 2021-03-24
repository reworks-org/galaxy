///
/// BaseTexture.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <vector>

#include <stb/stb_image_write.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/FileSystem.hpp"

#include "BaseTexture.hpp"

namespace galaxy
{
	namespace graphics
	{
		BaseTexture::BaseTexture(BaseTexture&& ba) noexcept
		{
			this->m_texture = ba.m_texture;
			this->m_width   = ba.m_width;
			this->m_height  = ba.m_height;

			ba.m_texture = 0;
			ba.m_width   = 0;
			ba.m_height  = 0;
		}

		BaseTexture& BaseTexture::operator=(BaseTexture&& ba) noexcept
		{
			if (this != &ba)
			{
				this->m_texture = ba.m_texture;
				this->m_width   = ba.m_width;
				this->m_height  = ba.m_height;

				ba.m_texture = 0;
				ba.m_width   = 0;
				ba.m_height  = 0;
			}

			return *this;
		}

		BaseTexture::~BaseTexture() noexcept
		{
			if (!m_shared)
			{
				glDeleteTextures(1, &m_texture);
			}
		}

		void BaseTexture::save(std::string_view file)
		{
			if (std::filesystem::path(file).extension().string() == ".png")
			{
				const auto path      = SL_HANDLE.vfs()->absolute(file);
				std::string path_str = "";
				if (path == std::nullopt)
				{
					path_str = std::filesystem::path(file).string();
				}
				else
				{
					path_str = path.value();
				}

				std::vector<unsigned int> pixels(m_width * m_height * 4, 0);

				glBindTexture(GL_TEXTURE_2D, m_texture);
				glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

				stbi_flip_vertically_on_write(true);
				stbi_write_png(path_str.c_str(), m_width, m_height, 4, pixels.data(), m_width * 4);

				glBindTexture(GL_TEXTURE_2D, 0);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Cannot save file: {0}, as extension is not PNG.", file);
			}
		}

		void BaseTexture::clamp_to_edge() noexcept
		{
			glBindTexture(GL_TEXTURE_2D, m_texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void BaseTexture::clamp_to_border() noexcept
		{
			glBindTexture(GL_TEXTURE_2D, m_texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void BaseTexture::set_repeated() noexcept
		{
			glBindTexture(GL_TEXTURE_2D, m_texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void BaseTexture::set_mirrored() noexcept
		{
			glBindTexture(GL_TEXTURE_2D, m_texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void BaseTexture::set_anisotropy(const unsigned int level) noexcept
		{
			glBindTexture(GL_TEXTURE_2D, m_texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, level);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		const int BaseTexture::get_width() const noexcept
		{
			return m_width;
		}

		const int BaseTexture::get_height() const noexcept
		{
			return m_height;
		}

		const int BaseTexture::get_aniso_level() noexcept
		{
			float ansio = 1.0f;
			glBindTexture(GL_TEXTURE_2D, m_texture);
			glGetTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, &ansio);
			glBindTexture(GL_TEXTURE_2D, 0);

			return ansio;
		}

		const unsigned int BaseTexture::gl_texture() const noexcept
		{
			return m_texture;
		}

		BaseTexture::BaseTexture() noexcept
		    : m_texture {0}, m_width {0}, m_height {0}, m_shared {false}
		{
			glGenTextures(1, &m_texture);
		}
	} // namespace graphics
} // namespace galaxy