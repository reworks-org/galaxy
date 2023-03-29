///
/// Framebuffer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "galaxy/core/Config.hpp"
#include "galaxy/core/ServiceLocator.hpp"

#include "Framebuffer.hpp"

constexpr const auto CLEAR_DEPTH = 1.0f;

namespace galaxy
{
	namespace graphics
	{
		Framebuffer::Framebuffer()
			: m_width {1}
			, m_height {1}
			, m_fbo {0}
			, m_depth_attachment {0}
			, m_depth_renderbuffer {0}
			, m_max_attachments {0}
			, m_clear_colour {0.0f, 0.0f, 0.0f, 0.0f}
		{
		}

		Framebuffer::Framebuffer(const int width, const int height)
			: m_width {1}
			, m_height {1}
			, m_fbo {0}
			, m_depth_attachment {0}
			, m_depth_renderbuffer {0}
			, m_max_attachments {0}
			, m_clear_colour {0.0f, 0.0f, 0.0f, 0.0f}
		{
			init(width, height);
		}

		Framebuffer::Framebuffer(Framebuffer&& f)
		{
			this->destroy();

			this->m_width              = f.m_width;
			this->m_height             = f.m_height;
			this->m_fbo                = f.m_fbo;
			this->m_depth_attachment   = f.m_depth_attachment;
			this->m_depth_renderbuffer = f.m_depth_renderbuffer;
			this->m_max_attachments    = f.m_max_attachments;

			this->m_attachments      = std::move(f.m_attachments);
			this->m_renderbuffers    = std::move(f.m_renderbuffers);
			this->m_used_attachments = std::move(f.m_used_attachments);
			this->m_clear_colour     = std::move(f.m_clear_colour);

			f.m_fbo                = 0;
			f.m_depth_attachment   = 0;
			f.m_depth_renderbuffer = 0;

			f.m_attachments.clear();
			f.m_renderbuffers.clear();
			f.m_used_attachments.clear();
		}

		Framebuffer& Framebuffer::operator=(Framebuffer&& f)
		{
			if (this != &f)
			{
				this->destroy();

				this->m_width              = f.m_width;
				this->m_height             = f.m_height;
				this->m_fbo                = f.m_fbo;
				this->m_depth_attachment   = f.m_depth_attachment;
				this->m_depth_renderbuffer = f.m_depth_renderbuffer;
				this->m_max_attachments    = f.m_max_attachments;

				this->m_attachments      = std::move(f.m_attachments);
				this->m_renderbuffers    = std::move(f.m_renderbuffers);
				this->m_used_attachments = std::move(f.m_used_attachments);
				this->m_clear_colour     = std::move(f.m_clear_colour);

				f.m_fbo                = 0;
				f.m_depth_attachment   = 0;
				f.m_depth_renderbuffer = 0;

				f.m_attachments.clear();
				f.m_renderbuffers.clear();
				f.m_used_attachments.clear();
			}

			return *this;
		}

		Framebuffer::~Framebuffer()
		{
			destroy();
		}

		void Framebuffer::init(const int width, const int height)
		{
			m_width  = width;
			m_height = height;

			glCreateFramebuffers(1, &m_fbo);

			auto max_attachments       = 0;
			auto max_drawn_attachments = 0;
			glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &max_attachments);
			glGetIntegerv(GL_MAX_DRAW_BUFFERS, &max_drawn_attachments);

			m_max_attachments = static_cast<unsigned int>(std::min(max_attachments, max_drawn_attachments));
		}

		void Framebuffer::add_colour_attachment(const bool high_precision)
		{
			if (m_used_attachments.size() < m_max_attachments)
			{
				auto& config = core::ServiceLocator<core::Config>::ref();

				GLuint texture = 0;
				GLenum gl_type = (high_precision == true) ? GL_FLOAT : GL_UNSIGNED_BYTE;

				glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, gl_type, nullptr);

				if (config.get<int>("trilinear_filtering", "graphics"))
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}
				else
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				}

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, static_cast<float>(config.get<int>("ansiotrophic_filtering", "graphics")));

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(m_used_attachments.size()), GL_TEXTURE_2D, texture, 0);
				m_used_attachments.push_back(GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(m_used_attachments.size()));
				m_attachments.push_back(std::make_pair(texture, gl_type));

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "No attachments left avaliable.");
			}
		}

		void Framebuffer::add_colour_renderbuffer(const int format)
		{
			if (m_used_attachments.size() < m_max_attachments)
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
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "No attachments left avaliable.");
			}
		}

		void Framebuffer::add_depth_attachment()
		{
			if (!((m_depth_attachment != 0) || (m_depth_renderbuffer != 0)))
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
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Framebuffer already has a depth component.");
			}
		}

		void Framebuffer::add_depth_renderbuffer()
		{
			if (!((m_depth_attachment != 0) || (m_depth_renderbuffer != 0)))
			{
				glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

				glGenRenderbuffers(1, &m_depth_renderbuffer);
				glBindRenderbuffer(GL_RENDERBUFFER, m_depth_renderbuffer);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth_renderbuffer);

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glBindRenderbuffer(GL_RENDERBUFFER, 0);
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Framebuffer already has a depth component.");
			}
		}

		unsigned int Framebuffer::add_storage_attachment()
		{
			if (m_used_attachments.size() < m_max_attachments)
			{
				GLuint texture = 0;

				glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, m_width, m_height, 0, GL_RED_INTEGER, GL_INT, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 1.0f);

				const auto next = static_cast<GLenum>(m_used_attachments.size());

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + next, GL_TEXTURE_2D, texture, 0);
				m_used_attachments.push_back(GL_COLOR_ATTACHMENT0 + next);
				m_attachments.push_back(std::make_pair(texture, GL_INT));

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glBindTexture(GL_TEXTURE_2D, 0);

				return next;
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "No attachments left avaliable.");
				return 0;
			}
		}

		void Framebuffer::create()
		{
			static GLint s_cur_fbo = 0;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &s_cur_fbo);

			glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
			add_drawbuffers();
			glBindFramebuffer(GL_FRAMEBUFFER, s_cur_fbo);
		}

		void Framebuffer::resize(const int width, const int height)
		{
			m_width  = width;
			m_height = height;

			static GLint s_cur_fbo = 0;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &s_cur_fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

			for (const auto& pair : m_attachments)
			{
				glBindTexture(GL_TEXTURE_2D, pair.first);
				if (pair.second == GL_INT)
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, m_width, m_height, 0, GL_RED_INTEGER, GL_INT, nullptr);
				}
				else
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, pair.second, nullptr);
				}
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

			add_drawbuffers();
			glBindFramebuffer(GL_FRAMEBUFFER, s_cur_fbo);
		}

		void Framebuffer::bind(const bool clear)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
			glViewport(0, 0, m_width, m_height);

			if (clear)
			{
				glClearColor(m_clear_colour[0], m_clear_colour[1], m_clear_colour[2], m_clear_colour[3]);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}
		}

		void Framebuffer::unbind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void Framebuffer::clear()
		{
			for (const auto index : m_used_attachments)
			{
				glClearNamedFramebufferfv(m_fbo, GL_COLOR, index, m_clear_colour.data());
			}

			glClearNamedFramebufferfv(m_fbo, GL_DEPTH, 0, &CLEAR_DEPTH);
		}

		void Framebuffer::clear(const unsigned int index)
		{
			glClearNamedFramebufferfv(m_fbo, GL_COLOR, index, m_clear_colour.data());
		}

		void Framebuffer::set_clear_colour(graphics::Colour& col)
		{
			m_clear_colour = col.normalized();
		}

		void Framebuffer::clear_storagebuffer(const unsigned int index, const int val)
		{
			glClearNamedFramebufferiv(m_fbo, GL_COLOR, index, &val);
		}

		int Framebuffer::read_storagebuffer(const unsigned int index, const int x, const int y)
		{
			static GLint s_cur_fbo = 0;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &s_cur_fbo);

			glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
			glReadBuffer(GL_COLOR_ATTACHMENT0 + index);

			int data = -1;
			glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &data);

			glBindFramebuffer(GL_FRAMEBUFFER, s_cur_fbo);
			return data;
		}

		int Framebuffer::get_width() const
		{
			return m_width;
		}

		int Framebuffer::get_height() const
		{
			return m_height;
		}

		unsigned int Framebuffer::get_depth_attachment() const
		{
			return m_depth_attachment;
		}

		unsigned int Framebuffer::get_depth_renderbuffer() const
		{
			return m_depth_renderbuffer;
		}

		std::vector<unsigned int> Framebuffer::get_attachments() const
		{
			std::vector<unsigned int> attachments;
			attachments.reserve(m_attachments.size());

			for (const auto& pair : m_attachments)
			{
				attachments.push_back(pair.first);
			}

			return attachments;
		}

		const std::vector<unsigned int>& Framebuffer::get_renderbuffers() const
		{
			return m_renderbuffers;
		}

		unsigned int Framebuffer::id() const
		{
			return m_fbo;
		}

		void Framebuffer::add_drawbuffers()
		{
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
					default:
						reason = "UNKNOWN";
						break;
				}

				GALAXY_LOG(GALAXY_FATAL, "Failed to complete framebuffer: {0}.", reason);
			}

			glClearColor(m_clear_colour[0], m_clear_colour[1], m_clear_colour[2], m_clear_colour[3]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void Framebuffer::destroy()
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
				m_depth_renderbuffer = 0;
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
	} // namespace graphics
} // namespace galaxy