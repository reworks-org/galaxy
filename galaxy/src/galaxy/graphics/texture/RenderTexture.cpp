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

#include "RenderTexture.hpp"

namespace galaxy
{
	namespace graphics
	{
		RenderTexture::RenderTexture() noexcept
		    : m_clear {true}, m_projection {1.0f}, m_framebuffer {0}, m_renderbuffer {0}
		{
			glGenFramebuffers(1, &m_framebuffer);
			glGenRenderbuffers(1, &m_renderbuffer);
		}

		RenderTexture::RenderTexture(const int width, const int height)
		    : m_clear {true}, m_projection {1.0f}, m_framebuffer {0}, m_renderbuffer {0}
		{
			glGenFramebuffers(1, &m_framebuffer);
			glGenRenderbuffers(1, &m_renderbuffer);

			create(width, height);
		}

		RenderTexture::RenderTexture(RenderTexture&& rt) noexcept
		    : BaseTexture {std::move(rt)}
		{
			this->m_clear        = rt.m_clear;
			this->m_projection   = std::move(rt.m_projection);
			this->m_framebuffer  = rt.m_framebuffer;
			this->m_renderbuffer = rt.m_renderbuffer;

			rt.m_framebuffer  = 0;
			rt.m_renderbuffer = 0;
		}

		RenderTexture& RenderTexture::operator=(RenderTexture&& rt) noexcept
		{
			if (this != &rt)
			{
				BaseTexture::operator=(std::move(rt));

				this->m_clear        = rt.m_clear;
				this->m_projection   = std::move(rt.m_projection);
				this->m_framebuffer  = rt.m_framebuffer;
				this->m_renderbuffer = rt.m_renderbuffer;

				rt.m_framebuffer  = 0;
				rt.m_renderbuffer = 0;
			}

			return *this;
		}

		RenderTexture::~RenderTexture() noexcept
		{
			glDeleteFramebuffers(1, &m_framebuffer);
			glDeleteRenderbuffers(1, &m_renderbuffer);
		}

		void RenderTexture::create(const int width, const int height)
		{
			m_width  = width;
			m_height = height;

			if (m_width < 1)
			{
				m_width = 1;
			}

			if (m_height < 1)
			{
				m_height = 1;
			}

			m_projection = glm::ortho(0.0f, static_cast<float>(m_width), static_cast<float>(m_height), 0.0f, -1.0f, 1.0f);

			// Framebuffer creation.
			glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
			glBindTexture(GL_TEXTURE_2D, m_texture);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			glGenerateMipmap(GL_TEXTURE_2D);

			// Generate mipmaps since it applies to the framebuffer, but leave AA and AT as post effects.
			if (SL_HANDLE.config()->get<bool>("trilinear-filtering"))
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, SL_HANDLE.config()->get<int>("ansio-filter"));

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);

			// Renderbuffer creation.
			glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderbuffer);

			const auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (status != GL_FRAMEBUFFER_COMPLETE)
			{
				std::string reason = "";
				switch (status)
				{
					case GL_FRAMEBUFFER_UNDEFINED:
						reason = "GL_FRAMEBUFFER_UNDEFINED";
						break;
					case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
						reason = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
						break;
					case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
						reason = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
						break;
					case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
						reason = "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
						break;
					case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
						reason = "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
						break;
					case GL_FRAMEBUFFER_UNSUPPORTED:
						reason = "GL_FRAMEBUFFER_UNSUPPORTED";
						break;
					case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
						reason = "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
						break;
					case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
						reason = "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
						break;
				}

				GALAXY_LOG(GALAXY_FATAL, "Failed to complete rendertexture framebuffer: {0}.", reason);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void RenderTexture::change_size(const int width, const int height)
		{
			glDeleteFramebuffers(1, &m_framebuffer);
			glDeleteRenderbuffers(1, &m_renderbuffer);

			glGenFramebuffers(1, &m_framebuffer);
			glGenRenderbuffers(1, &m_renderbuffer);

			create(width, height);
		}

		void RenderTexture::toggle_clearing(const bool clear)
		{
			m_clear = clear;
		}

		void RenderTexture::clear_framebuffer() noexcept
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void RenderTexture::bind() noexcept
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

			glViewport(0, 0, m_width, m_height);
			glEnable(GL_BLEND);

			if (m_clear)
			{
				glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}
		}

		void RenderTexture::unbind() noexcept
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void RenderTexture::set_projection(const float left, const float right, const float bottom, const float top) noexcept
		{
			m_projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		}

		const glm::mat4& RenderTexture::get_proj() noexcept
		{
			return m_projection;
		}
	} // namespace graphics
} // namespace galaxy