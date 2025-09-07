///
/// Texture2D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <stb_image.h>
#include <stb_image_write.h>

#include "galaxy/core/Config.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/utils/Globals.hpp"

#include "Texture2D.hpp"

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(push)
#pragma warning(disable : 26493)
#endif

namespace galaxy
{
	namespace graphics
	{
		Texture2D::Texture2D()
			: Texture {}
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
		}

		Texture2D::Texture2D(Texture2D&& t)
			: Texture {std::move(t)}
		{
		}

		Texture2D& Texture2D::operator=(Texture2D&& t)
		{
			if (this != &t)
			{
				Texture::operator=(std::move(t));
			}

			return *this;
		}

		Texture2D::~Texture2D()
		{
		}

		bool Texture2D::load(const std::string& file)
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

		bool Texture2D::load_mem(std::span<std::uint8_t> buffer)
		{
			auto result = false;

			auto& config = core::ServiceLocator<core::Config>::ref();

			stbi_set_flip_vertically_on_load(true);
			unsigned char* data = stbi_load_from_memory(buffer.data(), static_cast<int>(buffer.size_bytes()), &m_width, &m_height, nullptr, STBI_rgb_alpha);

			if (data)
			{
				glTextureStorage2D(m_id, 1, GL_RGBA8, m_width, m_height);
				glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, data);
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

				result = true;
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load texture from memory because '{0}'.", stbi_failure_reason());
			}

			stbi_image_free(data);
			return result;
		}

		void Texture2D::save(std::string_view file)
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

			glGetTextureImage(m_id, 0, GL_RGBA, GL_UNSIGNED_BYTE, static_cast<GLsizei>(pixels.size()), pixels.data());

			stbi_flip_vertically_on_write(true);

			int            len = 0;
			unsigned char* png = stbi_write_png_to_mem((const unsigned char*)pixels.data(), m_width * 4, m_width, m_height, 4, &len);

			if (!fs.write_raw(png, len, path.string()))
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to write '{0}' to disk.", path.string());
			}

			mi_free(png);
		}

		void Texture2D::recreate()
		{
			if (m_id != 0)
			{
				glDeleteTextures(1, &m_id);
				m_id = 0;
			}

			glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
		}

		void Texture2D::bind()
		{
			glBindTexture(GL_TEXTURE_2D, m_id);
		}

		void Texture2D::unbind()
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	} // namespace graphics
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(pop)
#endif
