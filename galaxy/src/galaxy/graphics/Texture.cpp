///
/// Texture.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <stb_image.h>
#include <stb_image_write.h>

#include "galaxy/core/Config.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Texture.hpp"

namespace galaxy
{
	namespace graphics
	{
		Texture::Texture() noexcept
			: m_width {0}
			, m_height {0}
			, m_texture {0}
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);
		}

		Texture::Texture(std::string_view file) noexcept
			: m_width {0}
			, m_height {0}
			, m_texture {0}
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);
			load(file);
		}

		Texture::Texture(Texture&& t) noexcept
		{
			if (this->m_texture != 0)
			{
				glDeleteTextures(1, &this->m_texture);
				this->m_texture = 0;
			}

			this->m_width   = t.m_width;
			this->m_height  = t.m_height;
			this->m_texture = t.m_texture;

			t.m_texture = 0;
		}

		Texture& Texture::operator=(Texture&& t) noexcept
		{
			if (this != &t)
			{
				if (this->m_texture != 0)
				{
					glDeleteTextures(1, &this->m_texture);
					this->m_texture = 0;
				}

				this->m_width   = t.m_width;
				this->m_height  = t.m_height;
				this->m_texture = t.m_texture;

				t.m_texture = 0;
			}

			return *this;
		}

		Texture::~Texture() noexcept
		{
			if (m_texture != 0)
			{
				glDeleteTextures(1, &m_texture);
				m_texture = 0;
			}
		}

		bool Texture::load(std::string_view file)
		{
			bool result = false;

			auto& fs     = core::ServiceLocator<fs::VirtualFileSystem>::ref();
			auto& config = core::ServiceLocator<core::Config>::ref();

			const auto file_info = fs.find(file);
			if (file_info.m_code == fs::FileCode::FOUND)
			{
				if (file_info.m_path.extension() == ".png")
				{
					stbi_set_flip_vertically_on_load(true);
					unsigned char* data = stbi_load(file_info.m_string.c_str(), &m_width, &m_height, nullptr, STBI_rgb_alpha);

					if (data)
					{
						glTextureStorage2D(m_texture, 1, GL_RGBA8, m_width, m_height);
						glTextureSubImage2D(m_texture, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, data);
						glGenerateTextureMipmap(m_texture);

						if (config.get<int>("trilinear_filtering", "graphics"))
						{
							glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
							glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						}
						else
						{
							glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
							glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
						}

						glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
						glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
						glTextureParameterf(m_texture, GL_TEXTURE_MAX_ANISOTROPY, static_cast<float>(config.get<int>("ansiotrophic_filtering", "graphics")));

						result = true;
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to load texture file '{0}' because '{1}'.", file, stbi_failure_reason());
					}

					stbi_image_free(data);
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Only PNG textures are supported, invalid texture '{0}'.", file);
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find texture from file '{0}'.");
			}

			return result;
		}

		bool Texture::load_mem(std::span<unsigned char> buffer)
		{
			bool result = false;

			auto& config = core::ServiceLocator<core::Config>::ref();

			stbi_set_flip_vertically_on_load(true);
			unsigned char* data = stbi_load_from_memory(buffer.data(), static_cast<int>(buffer.size_bytes()), &m_width, &m_height, nullptr, STBI_rgb_alpha);

			if (data)
			{
				glTextureStorage2D(m_texture, 1, GL_RGBA8, m_width, m_height);
				glTextureSubImage2D(m_texture, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, data);
				glGenerateTextureMipmap(m_texture);

				if (config.get<int>("trilinear_filtering", "graphics"))
				{
					glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}
				else
				{
					glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
					glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				}

				glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTextureParameterf(m_texture, GL_TEXTURE_MAX_ANISOTROPY, static_cast<float>(config.get<int>("ansiotrophic_filtering", "graphics")));

				result = true;
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load texture from memory because '{0}'.", stbi_failure_reason());
			}

			stbi_image_free(data);
			return result;
		}

		void Texture::save(std::string_view filepath)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			auto path = std::filesystem::path(filepath);
			auto full = (fs.root_path() / path.parent_path() / path.stem()).string();

			if (!full.ends_with(".png") || !full.ends_with(".PNG"))
			{
				full += ".png";
			}

			std::vector<unsigned int> pixels(m_width * m_height * 4, 0);

			glGetTextureImage(m_texture, 0, GL_RGBA, GL_UNSIGNED_BYTE, static_cast<GLsizei>(pixels.size()), pixels.data());

			stbi_flip_vertically_on_write(true);
			stbi_write_png(full.c_str(), m_width, m_height, 4, pixels.data(), m_width * 4);
		}

		void Texture::bind() noexcept
		{
			glBindTexture(GL_TEXTURE_2D, m_texture);
		}

		void Texture::unbind() noexcept
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void Texture::set_mode(const TextureModes mode) noexcept
		{
			glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, static_cast<GLint>(mode));
			glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, static_cast<GLint>(mode));
		}

		void Texture::set_filter(const TextureFilters filter) noexcept
		{
			switch (filter)
			{
				case TextureFilters::MIN_NEAREST:
					glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
					break;

				case TextureFilters::MIN_TRILINEAR:
					glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					break;

				case TextureFilters::MAG_NEAREST:
					glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					break;

				case TextureFilters::MAG_TRILINEAR:
					glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					break;
			}
		}

		void Texture::set_anisotropy(const int level) noexcept
		{
			glTextureParameterf(m_texture, GL_TEXTURE_MAX_ANISOTROPY, static_cast<float>(level));
		}

		int Texture::get_width() const noexcept
		{
			return m_width;
		}

		int Texture::get_height() const noexcept
		{
			return m_height;
		}

		float Texture::get_widthf() const noexcept
		{
			return static_cast<float>(m_width);
		}

		float Texture::get_heightf() const noexcept
		{
			return static_cast<float>(m_height);
		}

		int Texture::get_aniso_level() noexcept
		{
			auto ansio = 0.0f;
			glGetTextureParameterfv(m_texture, GL_TEXTURE_MAX_ANISOTROPY, &ansio);

			return static_cast<int>(std::trunc(ansio));
		}

		unsigned int Texture::handle() const noexcept
		{
			return m_texture;
		}
	} // namespace graphics
} // namespace galaxy