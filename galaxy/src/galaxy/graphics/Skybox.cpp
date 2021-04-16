///
/// Skybox.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/Config.hpp"
#include "galaxy/fs/FileSystem.hpp"

#include "Skybox.hpp"

namespace galaxy
{
	namespace graphics
	{
		Skybox::Skybox() noexcept
		    : m_texture {0}, m_width {0}, m_height {0}
		{
			glGenTextures(1, &m_texture);
			glGenBuffers(1, &m_vbo);
			glGenVertexArrays(1, &m_vao);
		}

		Skybox::Skybox(Skybox&& s) noexcept
		{
		}

		Skybox& Skybox::operator=(Skybox&& s) noexcept
		{
			if (this != &s)
			{
			}

			return *this;
		}

		Skybox::~Skybox() noexcept
		{
			glDeleteTextures(1, &m_texture);
			glDeleteBuffers(1, &m_vbo);
			glDeleteVertexArrays(1, &m_vao);
		}

		void Skybox::load(const std::array<std::string, 6>& faces)
		{
			bind();

			for (unsigned int i = 0; i < faces.size(); i++)
			{
				const auto path = SL_HANDLE.vfs()->absolute(faces[i]);
				if (path == std::nullopt)
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to find skybox face: {0}.", faces[i]);
				}
				else
				{
					unsigned char* data = stbi_load(path.value().c_str(), &m_width, &m_height, nullptr, STBI_rgb_alpha);

					if (data)
					{
						// Gen skybox into OpenGL.
						glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to load skybox face: {0}. {1}.", faces[i], stbi_failure_reason());
					}

					stbi_image_free(data);
				}
			}

			// Generate mipmaps.
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
			if (SL_HANDLE.config()->get<bool>("trilinear-filtering"))
			{
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}

			// Wrapping and ansiotrophy.
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY, 1.0f);

			unbind();
			create();
		}

		void Skybox::bind() noexcept
		{
			glBindVertexArray(m_vao);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
		}

		void Skybox::unbind() noexcept
		{
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}

		const int Skybox::get_width() const noexcept
		{
			return m_width;
		}

		const int Skybox::get_height() const noexcept
		{
			return m_height;
		}

		const unsigned int Skybox::gl_texture() const noexcept
		{
			return m_texture;
		}

		void Skybox::create()
		{
			// clang-format off
			constexpr const float vertices[] = {
				-1.0f,  1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				-1.0f,  1.0f, -1.0f,
				 1.0f,  1.0f, -1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				 1.0f, -1.0f,  1.0f
			};
			// clang-format on

			constexpr const auto vertices_size = sizeof(vertices);
			constexpr const auto float_size    = sizeof(float);

			glBindVertexArray(m_vao);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, vertices_size, &vertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * float_size, nullptr);

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	} // namespace graphics
} // namespace galaxy