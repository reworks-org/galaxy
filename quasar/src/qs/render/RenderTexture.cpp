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
		:m_framebufferID(0), m_textureID(0), m_depthBufferID(0), m_width(w), m_height(h)
	{
		glGenFramebuffers(1, &m_framebufferID);
		glGenTextures(1, &m_textureID);
		glGenRenderbuffers(1, &m_depthBufferID);

		// Temp bind the buffers to set some values up.
		activate();

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferID);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_textureID, 0);

		GLenum t_colAttachment[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, t_colAttachment);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			qs::Error::handle().callback("RenderTexture.cpp", 46, "Failed to create GL_FRAMEBUFFER!");
		}

		deactivate();

		//http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
		//m_shader.load(
		//"#version 330 core \n\nlayout(location = 0) \n\nout vec3 col;", "");
	}

	RenderTexture::~RenderTexture()
	{
		deactivate();
		
		glDeleteTextures(1, &m_textureID);
		glDeleteRenderbuffers(1, &m_depthBufferID);
		glDeleteFramebuffers(1, &m_framebufferID);
	}

	void RenderTexture::activate() noexcept
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufferID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		glViewport(0, 0, m_width, m_height);
	}

	void RenderTexture::deactivate() noexcept
	{
		glBindTexture(GL_TEXTURE_2D, NULL);
		glBindFramebuffer(GL_FRAMEBUFFER, NULL);
		glBindRenderbuffer(GL_RENDERBUFFER, NULL);
	}

	void RenderTexture::setRepeated() noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	void RenderTexture::setMirrored() noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	}

	void RenderTexture::clampToEdge() noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	void RenderTexture::clampToBorder(protostar::Colour& border) noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border.asFloats().data());
	}

	void RenderTexture::setMinifyFilter(const qs::Texture::Filter& filter)
	{
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		if (filter == qs::Texture::Filter::LINEAR)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		else if (filter == qs::Texture::Filter::NEAREST)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
	}

	void RenderTexture::setMagnifyFilter(const qs::Texture::Filter& filter)
	{
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		if (filter == qs::Texture::Filter::LINEAR)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else if (filter == qs::Texture::Filter::NEAREST)
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
}