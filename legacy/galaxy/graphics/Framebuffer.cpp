///
/// Framebuffer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/Config.hpp"

#include "Framebuffer.hpp"

namespace galaxy
{
	namespace graphics
	{
		Framebuffer::Framebuffer() noexcept
			: m_width {1}
			, m_height {1}
			, m_fbo {0}
			, m_depth_attachment {0}
			, m_depth_renderbuffer {0}
			, m_max_attachments {0}
		{
		}

		Framebuffer::Framebuffer(const int width, const int height) noexcept
			: m_fbo {0}
			, m_depth_attachment {0}
			, m_depth_renderbuffer {0}
			, m_max_attachments {0}
		{
			init(width, height);
		}

		Framebuffer::Framebuffer(Framebuffer&& f) noexcept
		{
			this->m_attachments        = std::move(f.m_attachments);
			this->m_depth_attachment   = f.m_depth_attachment;
			this->m_fbo                = f.m_fbo;
			this->m_height             = f.m_height;
			this->m_max_attachments    = f.m_max_attachments;
			this->m_used_attachments   = std::move(f.m_used_attachments);
			this->m_width              = f.m_width;
			this->m_depth_renderbuffer = f.m_depth_renderbuffer;
			this->m_renderbuffers      = std::move(f.m_renderbuffers);

			f.m_attachments.clear();
			f.m_depth_attachment = 0;
			f.m_fbo              = 0;
			f.m_used_attachments.clear();
			f.m_depth_renderbuffer = 0;
			f.m_renderbuffers.clear();
		}

		Framebuffer& Framebuffer::operator=(Framebuffer&& f) noexcept
		{
			if (this != &f)
			{
				this->m_attachments        = std::move(f.m_attachments);
				this->m_depth_attachment   = f.m_depth_attachment;
				this->m_fbo                = f.m_fbo;
				this->m_height             = f.m_height;
				this->m_max_attachments    = f.m_max_attachments;
				this->m_used_attachments   = std::move(f.m_used_attachments);
				this->m_width              = f.m_width;
				this->m_depth_renderbuffer = f.m_depth_renderbuffer;
				this->m_renderbuffers      = std::move(f.m_renderbuffers);

				f.m_attachments.clear();
				f.m_depth_attachment = 0;
				f.m_fbo              = 0;
				f.m_used_attachments.clear();
				f.m_depth_renderbuffer = 0;
				f.m_renderbuffers.clear();
			}

			return *this;
		}

		Framebuffer::~Framebuffer()
		{
			if (m_fbo != 0)
			{
				glDeleteFramebuffers(1, &m_fbo);
				m_fbo = 0;
			}

			if (m_depth_attachment != 0)
			{
				glDeleteTextures(1, &m_depth_attachment);
				m_depth_attachment = 0;
			}

			if (m_depth_renderbuffer != 0)
			{
				glDeleteRenderbuffers(1, &m_depth_renderbuffer);
			}

			for (auto& pair : m_attachments)
			{
				glDeleteTextures(1, &pair.first);
			}
			m_attachments.clear();

			if (m_renderbuffers.size() > 0)
			{
				glDeleteRenderbuffers(static_cast<GLsizei>(m_renderbuffers.size()), m_renderbuffers.data());
				m_renderbuffers.clear();
			}

			m_used_attachments.clear();
		}

		void Framebuffer::init(const int width, const int height) noexcept
		{
			m_width  = width;
			m_height = height;

			glGenFramebuffers(1, &m_fbo);

			auto max_attachments       = 0;
			auto max_drawn_attachments = 0;
			glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &max_attachments);
			glGetIntegerv(GL_MAX_DRAW_BUFFERS, &max_drawn_attachments);

			m_max_attachments = static_cast<unsigned int>(std::min(max_attachments, max_drawn_attachments));
		}

		void Framebuffer::add_colour_attachment(const bool high_precision)
		{
			if (m_used_attachments.size() >= m_max_attachments)
			{
				GALAXY_LOG(GALAXY_ERROR, "No colour attachments left avaliable.");
			}
			else
			{
				glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

				GLuint       texture = 0;
				const GLenum gl_type = (high_precision == true) ? GL_FLOAT : GL_UNSIGNED_BYTE;

				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, gl_type, nullptr);
				glGenerateMipmap(GL_TEXTURE_2D);

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

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, SL_HANDLE.config()->get<float>("ansio-filter"));

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(m_used_attachments.size()), GL_TEXTURE_2D, texture, 0);
				m_used_attachments.push_back(GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(m_used_attachments.size()));
				m_attachments.push_back(std::make_pair(texture, gl_type));

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}

		void Framebuffer::add_colour_renderbuffer(const int format)
		{
			if (m_used_attachments.size() >= m_max_attachments)
			{
				GALAXY_LOG(GALAXY_ERROR, "No colour attachments left avaliable.");
			}
			else
			{
				glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

				GLuint renderbuffer = 0;
				glGenRenderbuffers(1, &renderbuffer);
				glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
				glRenderbufferStorage(GL_RENDERBUFFER, format, m_width, m_height);

				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(m_used_attachments.size()), GL_RENDERBUFFER, renderbuffer);
				m_used_attachments.push_back(GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(m_used_attachments.size()));
				m_renderbuffers.push_back(renderbuffer);

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glBindRenderbuffer(GL_RENDERBUFFER, 0);
			}
		}

		void Framebuffer::add_depth_attachment()
		{
			if ((m_depth_attachment != 0) || (m_depth_renderbuffer != 0))
			{
				GALAXY_LOG(GALAXY_WARNING, "Already have a depth component.");
			}
			else
			{
				glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

				glGenTextures(1, &m_depth_attachment);
				glBindTexture(GL_TEXTURE_2D, m_depth_attachment);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 1.0f);

				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_attachment, 0);

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}

		void Framebuffer::add_depth_renderbuffer()
		{
			if ((m_depth_attachment != 0) || (m_depth_renderbuffer != 0))
			{
				GALAXY_LOG(GALAXY_WARNING, "Already have a depth component.");
			}
			else
			{
				glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

				glGenRenderbuffers(1, &m_depth_renderbuffer);
				glBindRenderbuffer(GL_RENDERBUFFER, m_depth_renderbuffer);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth_renderbuffer);

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glBindRenderbuffer(GL_RENDERBUFFER, 0);
			}
		}

		void Framebuffer::create()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
			glDrawBuffers(static_cast<GLsizei>(m_used_attachments.size()), m_used_attachments.data());

			const auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (status != GL_FRAMEBUFFER_COMPLETE)
			{
				std::string reason;
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

				GALAXY_LOG(GALAXY_FATAL, "Failed to complete framebuffer: {0}.", reason);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void Framebuffer::resize(const int width, const int height)
		{
			m_width  = width;
			m_height = height;

			glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

			for (const auto& pair : m_attachments)
			{
				glBindTexture(GL_TEXTURE_2D, pair.first);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, pair.second, nullptr);
				glGenerateMipmap(GL_TEXTURE_2D);
			}

			glBindTexture(GL_TEXTURE_2D, 0);

			for (const auto& renderbuffer : m_renderbuffers)
			{
				glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);

				GLint format = 0;
				glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_INTERNAL_FORMAT, &format);

				glRenderbufferStorage(GL_RENDERBUFFER, format, m_width, m_height);
			}

			glBindRenderbuffer(GL_RENDERBUFFER, 0);

			if (m_depth_attachment != 0)
			{
				glBindTexture(GL_TEXTURE_2D, m_depth_attachment);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);
				glBindTexture(GL_TEXTURE_2D, 0);
			}

			if (m_depth_renderbuffer != 0)
			{
				glBindRenderbuffer(GL_RENDERBUFFER, m_depth_renderbuffer);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
				glBindRenderbuffer(GL_RENDERBUFFER, 0);
			}

			create(); // unbinds framebuffer.
		}

		void Framebuffer::bind(const bool clear) noexcept
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
			glViewport(0, 0, m_width, m_height);

			if (clear)
			{
				glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}
		}

		void Framebuffer::unbind() noexcept
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void Framebuffer::clear() noexcept
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		const int Framebuffer::get_width() const noexcept
		{
			return m_width;
		}

		const int Framebuffer::get_height() const noexcept
		{
			return m_height;
		}

		const unsigned int Framebuffer::get_depth_attachment() const noexcept
		{
			return m_depth_attachment;
		}

		const unsigned int Framebuffer::get_depth_renderbuffer() const noexcept
		{
			return m_depth_renderbuffer;
		}

		std::vector<unsigned int> Framebuffer::get_attachments() const noexcept
		{
			std::vector<unsigned int> attachments;
			attachments.reserve(m_attachments.size());

			for (const auto& pair : m_attachments)
			{
				attachments.push_back(pair.first);
			}

			return attachments;
		}

		const std::vector<unsigned int>& Framebuffer::get_renderbuffers() const noexcept
		{
			return m_renderbuffers;
		}

		const unsigned int Framebuffer::get_fbo() const noexcept
		{
			return m_fbo;
		}
	} // namespace graphics
} // namespace galaxy