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
		:m_texture(), m_framebufferID(0), m_depthBufferID(0), m_width(w), m_height(h)
	{
		glGenFramebuffers(1, &m_framebufferID);
		glGenRenderbuffers(1, &m_depthBufferID);
		
		// Scoped for cleanup.
		{
			unsigned int tempID;
			glGenTextures(1, &tempID);
			m_texture.load(tempID);
		}
		
		// Temp bind the buffers to set some values up.
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufferID);

		m_texture.bind();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		m_texture.setMinifyFilter(qs::Texture::Filter::LINEAR);
		m_texture.setMagnifyFilter(qs::Texture::Filter::LINEAR);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_texture.getGLTexture(), 0);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferID);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			qs::Error::handle().callback("RenderTexture.cpp", 46, "Failed to create GL_FRAMEBUFFER!");
		}

		m_texture.unbind();
		glBindRenderbuffer(GL_RENDERBUFFER, NULL);
		glBindFramebuffer(GL_FRAMEBUFFER, NULL);

		m_projection = glm::ortho(0, m_width, m_height, 0, -1, 1);
	}

	RenderTexture::~RenderTexture() noexcept
	{
		// Texture is destroyed through its destructor.

		deactivate();
		
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
}