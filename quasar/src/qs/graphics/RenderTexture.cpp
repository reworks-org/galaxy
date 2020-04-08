///
/// RenderTexture.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "qs/utils/Error.hpp"
#include "qs/core/Window.hpp"
#include "qs/core/Shader.hpp"

#include "RenderTexture.hpp"

///
/// Core namespace.
///
namespace qs
{
	RenderTexture::RenderTexture(const int w, const int h)
		:m_sprite(), m_framebufferID(0), m_depthBufferID(0), m_width(w), m_height(h)
	{
		glGenFramebuffers(1, &m_framebufferID);
		glGenRenderbuffers(1, &m_depthBufferID);
		
		// Temp bind the buffers to set some values up.
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufferID);
		m_sprite.load(m_width, m_height); // Calls bind().
		m_sprite.create();
		m_sprite.activate();

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_sprite.getGLTexture(), 0);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferID);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			qs::Error::handle().callback("RenderTexture.cpp", 41, "Failed to create GL_FRAMEBUFFER!");
		}

		glBindTexture(GL_TEXTURE_2D, NULL);
		glBindRenderbuffer(GL_RENDERBUFFER, NULL);
		glBindFramebuffer(GL_FRAMEBUFFER, NULL);

		m_projection = glm::ortho(0, m_width, m_height, 0, -1, 1);
	}

	RenderTexture::~RenderTexture() noexcept
	{
		// Sprite2Ds destructor is called here.
		
		glDeleteRenderbuffers(1, &m_depthBufferID);
		glDeleteFramebuffers(1, &m_framebufferID);
	}

	void RenderTexture::activate(qs::Shader& shader) noexcept
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, m_width, m_height);

		shader.setUniform("u_projection", m_projection);
	}

	void RenderTexture::deactivate(qs::Window& window) noexcept
	{
		window.makeCurrent();
	}

	qs::Sprite2D& RenderTexture::getSprite() noexcept
	{
		return m_sprite;
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