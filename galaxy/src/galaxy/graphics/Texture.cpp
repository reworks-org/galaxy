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
		Texture::Texture()
			: m_width {0}
			, m_height {0}
			, m_texture {0}
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);
		}

		Texture::Texture(Texture&& t)
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

		Texture& Texture::operator=(Texture&& t)
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

		Texture::~Texture()
		{
			if (m_texture != 0)
			{
				glDeleteTextures(1, &m_texture);
				m_texture = 0;
			}
		}

		bool Texture::load(const std::string& file)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			auto data = fs.read_binary(file);
			if (!data.empty())
			{
				return load_mem(data);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to read '{0}' from vfs.", file);
			}

			return false;
		}

		bool Texture::load_mem(std::span<std::uint8_t> buffer)
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

		void Texture::load_raw(const int width, const int height, unsigned int storage_format, unsigned int pixel_format, unsigned int type, void* buffer)
		{
			recreate();

			m_width  = width;
			m_height = height;

			glTextureStorage2D(m_texture, 1, storage_format, m_width, m_height);
			glTextureSubImage2D(m_texture, 0, 0, 0, m_width, m_height, pixel_format, type, buffer);
			glGenerateTextureMipmap(m_texture);

			glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTextureParameterf(m_texture, GL_TEXTURE_MAX_ANISOTROPY, 1.0f);
		}

		void Texture::save(std::string_view file)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			auto path = std::filesystem::path(file);
			if (!path.is_absolute())
			{
				path = GALAXY_ROOT_DIR / path;
			}

			if (!path.has_extension())
			{
				path.replace_extension(".png");
			}

			if (!std::filesystem::exists(path.parent_path()))
			{
				std::filesystem::create_directories(path.parent_path());
			}

			meta::vector<unsigned int> pixels(static_cast<unsigned int>(m_width) * static_cast<unsigned int>(m_height) * 4u, 0);

			glGetTextureImage(m_texture, 0, GL_RGBA, GL_UNSIGNED_BYTE, static_cast<GLsizei>(pixels.size()), pixels.data());

			stbi_flip_vertically_on_write(true);

			int            len = 0;
			unsigned char* png = stbi_write_png_to_mem((const unsigned char*)pixels.data(), m_width * 4, m_width, m_height, 4, &len);

			if (!fs.write_raw(png, len, path.string()))
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to write '{0}' to disk.", path.string());
			}

			mi_free(png);
		}

		void Texture::recreate()
		{
			glDeleteTextures(1, &m_texture);
			glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);
		}

		void Texture::bind()
		{
			glBindTexture(GL_TEXTURE_2D, m_texture);
		}

		void Texture::unbind()
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void Texture::set_mode(const TextureModes mode)
		{
			glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, static_cast<GLint>(mode));
			glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, static_cast<GLint>(mode));
		}

		void Texture::set_filter(const TextureFilters filter)
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

		void Texture::set_anisotropy(const int level)
		{
			glTextureParameterf(m_texture, GL_TEXTURE_MAX_ANISOTROPY, std::clamp(static_cast<float>(level), 1.0f, 16.0f));
		}

		int Texture::get_width() const
		{
			return m_width;
		}

		int Texture::get_height() const
		{
			return m_height;
		}

		float Texture::get_widthf() const
		{
			return static_cast<float>(m_width);
		}

		float Texture::get_heightf() const
		{
			return static_cast<float>(m_height);
		}

		int Texture::get_aniso_level()
		{
			auto ansio = 0.0f;
			glGetTextureParameterfv(m_texture, GL_TEXTURE_MAX_ANISOTROPY, &ansio);

			return static_cast<int>(std::trunc(ansio));
		}

		unsigned int Texture::handle() const
		{
			return m_texture;
		}
	} // namespace graphics
} // namespace galaxy
