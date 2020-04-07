///
/// RenderTexture.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "qs/utils/Error.hpp"

#include "RenderTexture.hpp"

///
/// Core namespace.
///
namespace qs
{
	RenderTexture::RenderTexture(const int w, const int h)
		:m_texture(), m_framebufferID(0), m_depthBufferID(0), m_width(w), m_height(h), m_preserveTexture(true)
	{
		glGenFramebuffers(1, &m_framebufferID);
		glGenRenderbuffers(1, &m_depthBufferID);
		glGenTextures(1, &m_texture);
		
		// Temp bind the buffers to set some values up.
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufferID);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_texture, 0);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferID);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			qs::Error::handle().callback("RenderTexture.cpp", 45, "Failed to create GL_FRAMEBUFFER!");
		}

		glBindTexture(GL_TEXTURE_2D, NULL);
		glBindRenderbuffer(GL_RENDERBUFFER, NULL);
		glBindFramebuffer(GL_FRAMEBUFFER, NULL);

		m_projection = glm::ortho(0, m_width, m_height, 0, -1, 1);
	}

	RenderTexture::~RenderTexture() noexcept
	{
		deactivate();

		if (!m_preserveTexture)
		{
			glDeleteTextures(1, &m_texture);
		}
		
		glDeleteRenderbuffers(1, &m_depthBufferID);
		glDeleteFramebuffers(1, &m_framebufferID);
	}

	void RenderTexture::activate(qs::Shader& shader) noexcept
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		shader.setUniform("u_projection", m_projection);
	}

	void RenderTexture::deactivate() noexcept
	{
		glBindFramebuffer(GL_FRAMEBUFFER, NULL);
	}

	void RenderTexture::preserveTexture(const bool preserveTexture) noexcept
	{
		m_preserveTexture = preserveTexture;
	}

	const unsigned int RenderTexture::getTexture() const noexcept
	{
		return m_texture;
	}

	const int RenderTexture::getWidth() const noexcept
	{
		return m_width;
	}

	const int RenderTexture::getHeight() const noexcept
	{
		return m_height;
	}
}