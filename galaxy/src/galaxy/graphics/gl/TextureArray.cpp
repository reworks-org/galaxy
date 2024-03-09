///
/// TextureArray.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <stb_image.h>

#include "galaxy/core/Config.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "TextureArray.hpp"

namespace galaxy
{
	namespace graphics
	{
		TextureArray::TextureArray()
			: Texture {}
		{
			glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_id);
		}

		TextureArray::TextureArray(TextureArray&& t)
			: Texture {std::move(t)}
		{
		}

		TextureArray& TextureArray::operator=(TextureArray&& t)
		{
			if (this != &t)
			{
				Texture::operator=(std::move(t));
			}

			return *this;
		}

		TextureArray::~TextureArray()
		{
			if (!m_data.empty())
			{
				for (unsigned char* data : m_data)
				{
					stbi_image_free(data);
				}
			}
		}

		bool TextureArray::add(const std::string& file)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			auto data = fs.read_binary(file);
			if (!data.empty())
			{
				stbi_set_flip_vertically_on_load(true);
				m_data.emplace_back(
					stbi_load_from_memory(data.data(), static_cast<int>(data.size() * sizeof(std::uint8_t)), nullptr, nullptr, nullptr, STBI_rgb_alpha));
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to read '{0}' from vfs.", file);
			}

			return false;
		}

		void TextureArray::build(const int width, const int height)
		{
			if (!m_data.empty())
			{
				auto& config = core::ServiceLocator<core::Config>::ref();

				m_width  = width;
				m_height = height;

				glTextureStorage3D(m_id, 1, GL_RGBA8, m_width, m_height, static_cast<GLsizei>(m_data.size()));

				for (auto i = 0; i < m_data.size(); i++)
				{
					auto data = m_data[i];
					if (data != nullptr)
					{
						glTextureSubImage3D(m_id, 0, 0, 0, i, m_width, m_height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
						stbi_image_free(data);
					}
				}

				m_data = {};

				glGenerateTextureMipmap(m_id);

				if (config.get<int>("trilinear_filtering", "graphics"))
				{
					filter(TextureFilter::TRILINEAR);
				}
				else
				{
					filter(TextureFilter::NEAREST);
				}

				mode(TextureMode::CLAMP_TO_EDGE);
				anisotropy(config.get<int>("ansiotrophic_filtering", "graphics"));
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Tried to build an empty texture array.");
			}
		}

		bool TextureArray::build_from_tileset(const std::string& file, const int tile_size)
		{
			auto result = true;

			auto& fs     = core::ServiceLocator<fs::VirtualFileSystem>::ref();
			auto& config = core::ServiceLocator<core::Config>::ref();

			auto buffer = fs.read_binary(file);
			if (!buffer.empty())
			{
				stbi_set_flip_vertically_on_load(true);
				unsigned char* data =
					stbi_load_from_memory(buffer.data(), static_cast<int>(buffer.size() * sizeof(std::uint8_t)), &m_width, &m_height, nullptr, STBI_rgb_alpha);

				if (data)
				{
					const auto tiles_x    = m_width / tile_size;
					const auto tiles_y    = m_height / tile_size;
					const auto tile_count = tiles_x * tiles_y;

					glTextureStorage3D(m_id, 1, GL_RGBA8, tile_size, tile_size, tile_count);

					unsigned int tilesheet = 0;
					glCreateTextures(GL_TEXTURE_2D, 1, &tilesheet);
					glTextureStorage2D(tilesheet, 1, GL_RGBA8, m_width, m_height);
					glTextureSubImage2D(tilesheet, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, data);

					for (auto i = 0; i < tile_count; ++i)
					{
						const auto x = (i % tiles_x) * tile_size;
						const auto y = (i / tiles_x) * tile_size;

						glCopyImageSubData(tilesheet, GL_TEXTURE_2D, 0, x, y, 0, m_id, GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, tile_size, tile_size, 1);
					}

					glDeleteTextures(1, &tilesheet);
					glGenerateTextureMipmap(m_id);

					if (config.get<int>("trilinear_filtering", "graphics"))
					{
						filter(TextureFilter::TRILINEAR);
					}
					else
					{
						filter(TextureFilter::NEAREST);
					}

					mode(TextureMode::CLAMP_TO_EDGE);
					anisotropy(config.get<int>("ansiotrophic_filtering", "graphics"));
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to load texture from memory because '{0}'.", stbi_failure_reason());
					result = false;
				}

				stbi_image_free(data);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to read '{0}' from vfs.", file);
				result = false;
			}

			return result;
		}

		void TextureArray::bind()
		{
			glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);
		}

		void TextureArray::unbind()
		{
			glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		}
	} // namespace graphics
} // namespace galaxy
