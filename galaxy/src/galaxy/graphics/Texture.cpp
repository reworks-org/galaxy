///
/// Texture.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <vector>

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
		    : m_loaded {false}, m_width {0}, m_height {0}, m_path {""}, m_texture {0}
		{
			glGenTextures(1, &m_texture);
		}

		Texture::Texture(Texture&& t) noexcept
		{
			this->m_height  = t.m_height;
			this->m_loaded  = t.m_loaded;
			this->m_texture = t.m_texture;
			this->m_width   = t.m_width;
			this->m_path    = std::move(t.m_path);

			t.m_texture = 0;
		}

		Texture& Texture::operator=(Texture&& t) noexcept
		{
			if (this != &t)
			{
				this->m_height  = t.m_height;
				this->m_loaded  = t.m_loaded;
				this->m_texture = t.m_texture;
				this->m_width   = t.m_width;
				this->m_path    = std::move(t.m_path);

				t.m_texture = 0;
			}

			return *this;
		}

		Texture::~Texture() noexcept
		{
			if (m_texture != 0)
			{
				glDeleteTextures(1, &m_texture);
			}
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
				m_path = path.value();

				glBindTexture(GL_TEXTURE_2D, m_texture);

				stbi_set_flip_vertically_on_load(true);
				unsigned char* data = stbi_load(m_path.c_str(), &m_width, &m_height, nullptr, STBI_rgb_alpha);

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
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, static_cast<float>(SL_HANDLE.config()->get<int>("ansio-filter")));

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
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, static_cast<float>(SL_HANDLE.config()->get<int>("ansio-filter")));

				m_loaded = true;
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load texture from memory: {0}.", stbi_failure_reason());
			}

			stbi_image_free(data);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void Texture::save(std::string_view file)
		{
			const auto abs = SL_HANDLE.vfs()->absolute(file);

			std::string path_str;
			if (abs == std::nullopt)
			{
				path_str = static_cast<std::string>(file);
			}
			else
			{
				path_str = abs.value();
			}

			const auto path = std::filesystem::path(path_str);
			const auto full = (path.parent_path() / path.stem()) += ".png";

			std::vector<unsigned int> pixels(m_width * m_height * 4, 0);

			glBindTexture(GL_TEXTURE_2D, m_texture);
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

			stbi_flip_vertically_on_write(true);
			stbi_write_png(full.string().c_str(), m_width, m_height, 4, pixels.data(), m_width * 4);

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

		void Texture::set_anisotropy(const float level) noexcept
		{
			glBindTexture(GL_TEXTURE_2D, m_texture);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, level);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		const bool Texture::is_loaded() const noexcept
		{
			return m_loaded;
		}

		const int Texture::get_width() const noexcept
		{
			return m_width;
		}

		const int Texture::get_height() const noexcept
		{
			return m_height;
		}

		const int Texture::get_aniso_level() noexcept
		{
			float ansio = 1.0f;
			glBindTexture(GL_TEXTURE_2D, m_texture);
			glGetTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, &ansio);
			glBindTexture(GL_TEXTURE_2D, 0);

			return ansio;
		}

		const std::string& Texture::get_filepath() const noexcept
		{
			return m_path;
		}

		const unsigned int Texture::gl_texture() const noexcept
		{
			return m_texture;
		}
	} // namespace graphics
} // namespace galaxy