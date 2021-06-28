///
/// RenderTexture.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/Config.hpp"
#include "galaxy/fs/FileSystem.hpp"

#include "RenderTexture.hpp"

#define ORTHO_NEAR      0.0f
#define ORTHO_FAR_24BIT 16777215.0f

namespace galaxy
{
	namespace graphics
	{
		RenderTexture::RenderTexture() noexcept
		    : m_projection {1.0f}
		{
		}

		RenderTexture::RenderTexture(const int width, const int height)
		    : m_projection {1.0f}
		{
			create(width, height);
		}

		RenderTexture::RenderTexture(RenderTexture&& rt) noexcept
		{
			this->m_framebuffer = std::move(rt.m_framebuffer);
			this->m_projection  = std::move(rt.m_projection);
		}

		RenderTexture& RenderTexture::operator=(RenderTexture&& rt) noexcept
		{
			if (this != &rt)
			{
				this->m_framebuffer = std::move(rt.m_framebuffer);
				this->m_projection  = std::move(rt.m_projection);
			}

			return *this;
		}

		void RenderTexture::create(int width, int height)
		{
			width  = std::max(1, width);
			height = std::max(1, height);

			m_projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, ORTHO_NEAR, ORTHO_FAR_24BIT);

			m_framebuffer.init(width, height);
			m_framebuffer.add_colour_attachment(GL_RGBA8, false);
			m_framebuffer.add_depth_renderbuffer();
			m_framebuffer.create();
		}

		void RenderTexture::save(std::string_view file)
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

			std::vector<unsigned int> pixels(m_framebuffer.get_width() * m_framebuffer.get_height() * 4, 0);

			glBindTexture(GL_TEXTURE_2D, get_texture());
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

			stbi_flip_vertically_on_write(true);
			stbi_write_png(full.string().c_str(), m_framebuffer.get_width(), m_framebuffer.get_height(), 4, pixels.data(), m_framebuffer.get_width() * 4);

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void RenderTexture::resize(int width, int height)
		{
			width  = std::max(1, width);
			height = std::max(1, height);

			m_projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, ORTHO_NEAR, ORTHO_FAR_24BIT);
			m_framebuffer.resize(width, height);
		}

		void RenderTexture::bind() noexcept
		{
			m_framebuffer.bind();
		}

		void RenderTexture::unbind() noexcept
		{
			m_framebuffer.unbind();
		}

		void RenderTexture::set_projection(const float left, const float right, const float bottom, const float top) noexcept
		{
			m_projection = glm::ortho(left, right, bottom, top, ORTHO_NEAR, ORTHO_FAR_24BIT);
		}

		const int RenderTexture::get_width() const noexcept
		{
			return m_framebuffer.get_width();
		}

		const int RenderTexture::get_height() const noexcept
		{
			return m_framebuffer.get_height();
		}

		const glm::mat4& RenderTexture::get_proj() noexcept
		{
			return m_projection;
		}

		const unsigned int RenderTexture::get_texture() const noexcept
		{
			// This works since the rendertexture will only ever have 1 attachment.
			return m_framebuffer.get_attachments()[0];
		}
	} // namespace graphics
} // namespace galaxy