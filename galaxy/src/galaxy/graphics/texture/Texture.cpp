///
/// Texture.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/Config.hpp"
#include "galaxy/fs/FileSystem.hpp"

#include "Texture.hpp"

namespace galaxy
{
	namespace graphics
	{
		Texture::Texture() noexcept
		    : BaseTexture {}, m_loaded {false}
		{
		}

		Texture::Texture(Texture&& t) noexcept
		    : BaseTexture {std::move(t)}
		{
			this->m_loaded = t.m_loaded;
		}

		Texture& Texture::operator=(Texture&& t) noexcept
		{
			if (this != &t)
			{
				BaseTexture::operator=(std::move(t));

				this->m_loaded = t.m_loaded;
			}

			return *this;
		}

		void Texture::load(std::string_view file)
		{
			const auto path = SL_HANDLE.vfs()->absolute(file);
			if (path == std::nullopt)
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find texture: {0}.", file);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, m_texture);

				stbi_set_flip_vertically_on_load(true);
				unsigned char* data = stbi_load(path.value().c_str(), &m_width, &m_height, nullptr, STBI_rgb_alpha);

				if (data)
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
					glGenerateMipmap(GL_TEXTURE_2D);

					if (SL_HANDLE.config()->get<bool>("trilinear-filtering"))
					{
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					}
					else
					{
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					}

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 1);

					clamp_to_edge();
					m_loaded = true;
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to load texture: {0}. {1}.", file, stbi_failure_reason());
				}

				stbi_image_free(data);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}

		void Texture::load_mem(std::span<unsigned char> buffer)
		{
			glBindTexture(GL_TEXTURE_2D, m_texture);

			stbi_set_flip_vertically_on_load(true);
			unsigned char* data = stbi_load_from_memory(buffer.data(), static_cast<int>(buffer.size_bytes()), &m_width, &m_height, nullptr, STBI_rgb_alpha);

			if (data)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);

				if (SL_HANDLE.config()->get<bool>("trilinear-filtering"))
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}
				else
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				}

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 1);

				m_loaded = true;
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load texture from memory: {0}.", stbi_failure_reason());
			}

			stbi_image_free(data);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void Texture::load(const unsigned int id, const int width, const int height) noexcept
		{
			// Clear out old data first.
			if (m_texture != id)
			{
				glDeleteTextures(1, &m_texture);
			}

			m_texture = id;
			m_width   = width;
			m_height  = height;

			m_shared = true;
			m_loaded = true;
		}

		void Texture::bind() noexcept
		{
			glBindTexture(GL_TEXTURE_2D, m_texture);
		}

		void Texture::unbind() noexcept
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		const bool Texture::is_loaded() const noexcept
		{
			return m_loaded;
		}
	} // namespace graphics
} // namespace galaxy