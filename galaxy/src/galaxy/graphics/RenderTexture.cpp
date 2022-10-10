///
/// RenderTexture.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/utils/Globals.hpp"

#include "RenderTexture.hpp"

namespace galaxy
{
	namespace graphics
	{
		RenderTexture::RenderTexture() noexcept
			: m_projection {GALAXY_IDENTITY_MATRIX}
		{
		}

		RenderTexture::RenderTexture(const int width, const int height)
			: m_projection {GALAXY_IDENTITY_MATRIX}
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

			m_projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);

			m_framebuffer.init(width, height);
			m_framebuffer.add_colour_attachment(false);
			m_framebuffer.add_depth_renderbuffer();
			m_framebuffer.create();
		}

		void RenderTexture::resize(int width, int height)
		{
			width  = std::max(1, width);
			height = std::max(1, height);

			m_projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
			m_framebuffer.resize(width, height);
		}

		void RenderTexture::save(std::string_view filepath)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			auto path = std::filesystem::path(filepath);

			auto full_path = fs.root_path() / path.parent_path() / path.stem();
			auto full      = full_path.string();

			if (!std::filesystem::exists(full_path.parent_path()))
			{
				std::filesystem::create_directories(full_path.parent_path());
			}

			if (!full.ends_with(".png") || !full.ends_with(".PNG"))
			{
				full += ".png";
			}

			std::vector<unsigned int> pixels(get_width() * get_height() * 4, 0);

			glGetTextureImage(get_texture(), 0, GL_RGBA, GL_UNSIGNED_BYTE, static_cast<GLsizei>(pixels.size()), pixels.data());

			stbi_flip_vertically_on_write(true);
			stbi_write_png(full.c_str(), get_width(), get_height(), 4, pixels.data(), get_width() * 4);
		}

		void RenderTexture::bind(const bool clear) noexcept
		{
			m_framebuffer.bind(clear);
		}

		void RenderTexture::unbind() noexcept
		{
			m_framebuffer.unbind();
		}

		void RenderTexture::clear() noexcept
		{
			m_framebuffer.clear();
		}

		void RenderTexture::set_projection(const float left, const float right, const float bottom, const float top) noexcept
		{
			m_projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		}

		int RenderTexture::get_width() const noexcept
		{
			return m_framebuffer.get_width();
		}

		int RenderTexture::get_height() const noexcept
		{
			return m_framebuffer.get_height();
		}

		glm::mat4& RenderTexture::get_proj() noexcept
		{
			return m_projection;
		}

		Framebuffer& RenderTexture::get_framebuffer() noexcept
		{
			return m_framebuffer;
		}

		unsigned int RenderTexture::get_texture() const noexcept
		{
			// This works since the rendertexture will only ever have 1 attachment.
			return m_framebuffer.get_attachments()[0];
		}
	} // namespace graphics
} // namespace galaxy