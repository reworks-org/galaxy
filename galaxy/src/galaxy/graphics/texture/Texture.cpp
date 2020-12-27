///
/// Texture.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include "galaxy/error/Log.hpp"

#include "Texture.hpp"

namespace galaxy
{
	namespace graphics
	{
		Texture::Texture()
		    : BaseTexture {}
		{
		}

		Texture::~Texture()
		{
		}

		void Texture::load(std::string_view file)
		{
			// Generate texture in OpenGL and bind to 2D texture.
			glBindTexture(GL_TEXTURE_2D, m_texture);

			stbi_set_flip_vertically_on_load(true);

			auto dest           = std::filesystem::path {file};
			unsigned char* data = stbi_load(dest.string().c_str(), &m_width, &m_height, nullptr, STBI_rgb_alpha);

			if (data)
			{
				// Gen texture into OpenGL.
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

				// Ansiotropic filtering.
				set_anisotropy(0);

				// Linear filtering for non-pixel as default.
				set_minify_filter(TextureFilter::LINEAR);
				set_magnify_filter(TextureFilter::LINEAR);

				// Default clamp to edge.
				clamp_to_edge();
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load texture: {0}. {1}.", file, stbi_failure_reason());
			}

			stbi_image_free(data);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void Texture::load_mem(std::span<unsigned char> buffer)
		{
			// Generate texture in OpenGL and bind to 2D texture.
			glBindTexture(GL_TEXTURE_2D, m_texture);

			stbi_set_flip_vertically_on_load(true);
			unsigned char* data = stbi_load_from_memory(buffer.data(), static_cast<int>(buffer.size_bytes()), &m_width, &m_height, nullptr, STBI_rgb_alpha);

			if (data)
			{
				// Gen texture into OpenGL.
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

				// Ansiotropic filtering.
				set_anisotropy(0);

				// Set filtering. When minimizing texture, linear interpolate, else nearest for nice pixel 2d art look.
				set_minify_filter(TextureFilter::LINEAR);

				// Set interpolation for mipmapping.
				set_magnify_filter(TextureFilter::LINEAR);

				// Default clamp to edge.
				clamp_to_edge();
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load texture from memory: {0}.", stbi_failure_reason());
			}

			stbi_image_free(data);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void Texture::load(const unsigned int id, const int width, const int height)
		{
			m_texture = id;
			m_width   = width;
			m_height  = height;
		}

		void Texture::load(int level, int internalformat, int width, int height, int border, unsigned int format, unsigned int type, const void* pixels)
		{
			m_width  = width;
			m_height = height;

			// Generate texture in OpenGL and bind to 2D texture.
			glBindTexture(GL_TEXTURE_2D, m_texture);

			// Gen texture into OpenGL.
			glTexImage2D(GL_TEXTURE_2D, level, internalformat, width, height, border, format, type, pixels);

			// Ansiotropic filtering.
			set_anisotropy(0);

			// Set filtering. When minimizing texture, linear interpolate, else nearest for nice pixel 2d art look.
			set_minify_filter(TextureFilter::LINEAR);

			// Set interpolation for mipmapping.
			set_magnify_filter(TextureFilter::LINEAR);

			// Default clamp to edge.
			clamp_to_edge();

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void Texture::bind()
		{
			glBindTexture(GL_TEXTURE_2D, m_texture);
		}

		void Texture::unbind()
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	} // namespace graphics
} // namespace galaxy