///
/// Texture.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <glad/glad.h>
#include <stb_image.h>
#include <stb_image_write.h>

#include "galaxy/core/Settings.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/graphics/gl/GLEnums.hpp"
#include "galaxy/graphics/gl/Sampler.hpp"
#include "galaxy/logging/Log.hpp"
#include "galaxy/platform/Pragma.hpp"

#include "Texture.hpp"

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(push)
#pragma warning(disable : 26493)
#endif

namespace galaxy
{
	Texture::Texture()
		: m_id {0}
		, m_handle {0}
		, m_width {0}
		, m_height {0}
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
	}

	Texture::Texture(Texture&& t) noexcept
	{
		this->destroy();

		this->m_id     = t.m_id;
		this->m_handle = t.m_handle;
		this->m_width  = t.m_width;
		this->m_height = t.m_height;

		t.m_id     = 0;
		t.m_handle = 0;
	}

	Texture& Texture::operator=(Texture&& t) noexcept
	{
		if (this != &t)
		{
			this->destroy();

			this->m_id     = t.m_id;
			this->m_handle = t.m_handle;
			this->m_width  = t.m_width;
			this->m_height = t.m_height;

			t.m_id     = 0;
			t.m_handle = 0;
		}

		return *this;
	}

	Texture::~Texture()
	{
		destroy();
	}

	bool Texture::load(const std::string& file)
	{
		auto& fs = entt::locator<VirtualFileSystem>::value();

		auto data = fs.read_binary(file);
		return load_mem(data);
	}

	bool Texture::load_mem(std::span<std::uint8_t> buffer)
	{
		auto result = true;

		if (!buffer.empty())
		{
			stbi_set_flip_vertically_on_load(true);
			unsigned char* data = stbi_load_from_memory(buffer.data(), static_cast<int>(buffer.size_bytes()), &m_width, &m_height, nullptr, STBI_rgb_alpha);

			if (data)
			{
				glTextureStorage2D(m_id, 1, GL_RGBA8, m_width, m_height);
				glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, data);

				if (Settings::mipmap())
				{
					glGenerateTextureMipmap(m_id);
				}

				m_handle = glGetTextureSamplerHandleARB(m_id, entt::locator<Sampler>::value().id());
				glMakeTextureHandleResidentARB(m_handle);
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
			GALAXY_LOG(GALAXY_ERROR, "Tried to pass empty buffer to loading texture from memory.");
			result = false;
		}

		return result;
	}

	void Texture::save(std::string_view file)
	{
		auto& fs = entt::locator<VirtualFileSystem>::value();

		auto path = std::filesystem::path(file);
		if (!path.is_absolute())
		{
			path = Settings::root_dir() / path;
		}

		if (!path.has_extension())
		{
			path.replace_extension(".png");
		}

		if (!std::filesystem::exists(path.parent_path()))
		{
			std::filesystem::create_directories(path.parent_path());
		}

		std::vector<unsigned int> pixels(static_cast<unsigned int>(m_width) * static_cast<unsigned int>(m_height) * 4u, 0);

		glGetTextureImage(m_id, 0, GL_RGBA, GL_UNSIGNED_BYTE, static_cast<GLsizei>(pixels.size()), pixels.data());

		stbi_flip_vertically_on_write(true);

		int len = 0;

		unsigned char* png = stbi_write_png_to_mem((const unsigned char*)pixels.data(), m_width * 4, m_width, m_height, 4, &len);

		if (!fs.write_raw(png, len, path.string()))
		{
			GALAXY_LOG(GALAXY_ERROR, "Failed to write '{0}' to disk.", path.string());
		}

		std::free(png);
	}

	TextureView Texture::get_view(const unsigned int minlevel, const unsigned int numlevels, const unsigned int minlayer, const unsigned int numlayers) const noexcept
	{
		return {m_id, minlevel, numlevels, minlayer, numlayers};
	}

	void Texture::bind() const noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

	void Texture::unbind() const noexcept
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::destroy()
	{
		if (m_handle != 0)
		{
			glMakeTextureHandleNonResidentARB(m_handle);
			m_handle = 0;
		}

		if (m_id != 0)
		{
			glDeleteTextures(1, &m_id);
			m_id = 0;
		}
	}

	float Texture::width() const noexcept
	{
		return static_cast<float>(m_width);
	}

	float Texture::height() const noexcept
	{
		return static_cast<float>(m_height);
	}

	unsigned int Texture::id() const noexcept
	{
		return m_id;
	}

	std::uint64_t Texture::handle() const noexcept
	{
		return m_handle;
	}
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(pop)
#endif
