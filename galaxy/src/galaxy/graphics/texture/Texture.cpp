///
/// Texture.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

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
		    : BaseTexture {}
		{
		}

		Texture::Texture(Texture&& t) noexcept
		    : BaseTexture {std::move(t)}
		{
		}

		Texture& Texture::operator=(Texture&& t) noexcept
		{
			if (this != &t)
			{
				BaseTexture::operator=(std::move(t));
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
					// Gen texture into OpenGL.
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
					glGenerateMipmap(GL_TEXTURE_2D);

					if (SL_HANDLE.config()->get<bool>("trilinear-filtering"))
					{
						set_minify_filter<TrilinearMipmapFilter>();
						set_magnify_filter<BilinearTexFilter>();
					}
					else
					{
						set_minify_filter<NearestMipmapFilter>();
						set_magnify_filter<NearestTexFilter>();
					}

					clamp_to_border();
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
				// Gen texture into OpenGL.
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);

				if (SL_HANDLE.config()->get<bool>("trilinear-filtering"))
				{
					set_minify_filter<TrilinearMipmapFilter>();
					set_magnify_filter<BilinearTexFilter>();
				}
				else
				{
					set_minify_filter<NearestMipmapFilter>();
					set_magnify_filter<NearestTexFilter>();
				}

				clamp_to_border();
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
			m_texture = id;
			m_width   = width;
			m_height  = height;
		}

		void Texture::bind() noexcept
		{
			glBindTexture(GL_TEXTURE_2D, m_texture);
		}

		void Texture::unbind() noexcept
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	} // namespace graphics
} // namespace galaxy