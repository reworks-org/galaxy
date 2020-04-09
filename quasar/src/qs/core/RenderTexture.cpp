///
/// RenderTexture.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>
#include <stb_image.h>
#include <stb_image_write.h>

#include "qs/utils/Error.hpp"
#include "qs/core/Window.hpp"
#include "qs/core/Shader.hpp"

#include "RenderTexture.hpp"

///
/// Core namespace.
///
namespace qs
{
	RenderTexture::RenderTexture() noexcept
		:m_texture(0), m_framebuffer(0), m_renderbuffer(0), m_width(0), m_height(0)
	{
		glGenFramebuffers(1, &m_framebuffer);
		glGenTextures(1, &m_texture);
		glGenRenderbuffers(1, &m_renderbuffer);
	}

	RenderTexture::RenderTexture(const int w, const int h)
		:m_texture(0), m_framebuffer(0), m_renderbuffer(0), m_width(0), m_height(0)
	{
		glGenFramebuffers(1, &m_framebuffer);
		glGenTextures(1, &m_texture);
		glGenRenderbuffers(1, &m_renderbuffer);

		create(w, h);
	}

	RenderTexture::~RenderTexture()
	{
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_TEXTURE_2D, 0);

		glDeleteRenderbuffers(1, &m_renderbuffer);
		glDeleteFramebuffers(1, &m_framebuffer);
		glDeleteTextures(1, &m_texture);
	}

	void RenderTexture::create(const int w, const int h) noexcept
	{
		m_width = w;
		m_height = h;

		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderbuffer);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			qs::Error::handle().callback("RenderTexture.cpp", 31, "Failed to create GL_FRAMEBUFFER!");
		}

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void RenderTexture::save(const std::string& path) noexcept
	{
		if (!path.empty())
		{
			std::vector<unsigned int> pixels(m_width * m_height * 4, 0);

			glBindTexture(GL_TEXTURE_2D, m_texture);
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
			stbi_write_png(path.c_str(), m_width, m_height, 4, pixels.data(), m_width * 4);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	void RenderTexture::activate(qs::Shader& shader) noexcept
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		glViewport(0, 0, m_width, m_height);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
	}

	void RenderTexture::deactivate(qs::Window& window) noexcept
	{
		glFlush();
		window.makeCurrent();
	}

	void RenderTexture::setRepeated() noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	void RenderTexture::setMirrored() noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	}

	void RenderTexture::clampToEdge() noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	void RenderTexture::clampToBorder(protostar::Colour& border) noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border.asFloats().data());
	}

	void RenderTexture::setMinifyFilter(const qs::TextureFilter& filter) noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
		if (filter == qs::TextureFilter::LINEAR)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		else if (filter == qs::TextureFilter::NEAREST)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
	}

	void RenderTexture::setMagnifyFilter(const qs::TextureFilter& filter) noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
		if (filter == qs::TextureFilter::LINEAR)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else if (filter == qs::TextureFilter::NEAREST)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
	}

	const int RenderTexture::getWidth() const noexcept
	{
		return m_width;
	}

	const int RenderTexture::getHeight() const noexcept
	{
		return m_height;
	}

	const unsigned int RenderTexture::getGLTexture() const noexcept
	{
		return m_texture;
	}
}