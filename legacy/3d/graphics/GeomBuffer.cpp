///
/// GeomBuffer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/Config.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/graphics/shaders/GeomBuffer.hpp"

#include "GeomBuffer.hpp"

namespace galaxy
{
	namespace graphics
	{
		GeomBuffer::GeomBuffer()
		    : m_max_attachments {0}, m_width {0}, m_height {0}, m_fbo {0}, m_depth_tex {0}
		{
		}

		GeomBuffer::~GeomBuffer()
		{
			destroy();
		}

		void GeomBuffer::init(const int width, const int height)
		{
			m_width  = width;
			m_height = height;

			glGenFramebuffers(1, &m_fbo);

			auto max_attachments       = 0;
			auto max_drawn_attachments = 0;
			glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &max_attachments);
			glGetIntegerv(GL_MAX_DRAW_BUFFERS, &max_drawn_attachments);

			m_max_attachments = std::min(max_attachments, max_drawn_attachments);

			m_shader.load_raw(shaders::geom_vert, shaders::geom_frag);
		}

		void GeomBuffer::add_attachments(const unsigned int count, const AttachmentType type)
		{
			for (unsigned int counter = 0; counter < count; counter++)
			{
				if (m_attachments.size() >= m_max_attachments)
				{
					GALAXY_LOG(GALAXY_ERROR, "No colour attachments left avaliable.");
				}
				else
				{
					const GLint format   = (type == AttachmentType::FLOAT) ? GL_RGBA16F : GL_RGBA;
					const GLenum gl_type = (type == AttachmentType::FLOAT) ? GL_FLOAT : GL_UNSIGNED_BYTE;

					unsigned int id;
					glGenTextures(1, &id);
					glBindTexture(GL_TEXTURE_2D, id);
					glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, GL_RGBA, gl_type, nullptr);
					glGenerateMipmap(GL_TEXTURE_2D);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, static_cast<float>(SL_HANDLE.config()->get<int>("ansio-filter")));

					m_attachments.push_back(id);
				}
			}

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void GeomBuffer::create()
		{
			if (m_attachments.empty())
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to create geometry buffer with no attachments.");
			}
			else
			{
				glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
				m_shader.bind();

				for (unsigned int index = 0; index < m_attachments.size(); index++)
				{
					glBindTexture(GL_TEXTURE_2D, m_attachments[index]);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, m_attachments[index], 0);

					m_used_colour_attachments.push_back(GL_COLOR_ATTACHMENT0 + index);
				}

				glDrawBuffers(m_used_colour_attachments.size(), m_used_colour_attachments.data());

				glGenTextures(1, &m_depth_tex);
				glBindTexture(GL_TEXTURE_2D, m_depth_tex);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 1.0f);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_tex, 0);

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

					GALAXY_LOG(GALAXY_FATAL, "Failed to complete geometry framebuffer: {0}.", reason);
				}

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glBindTexture(GL_TEXTURE_2D, 0);
				m_shader.unbind();
			}
		}

		void GeomBuffer::resize(const int width, const int height)
		{
			m_width  = width;
			m_height = height;

			glDeleteFramebuffers(1, &m_fbo);
			glDeleteTextures(1, &m_depth_tex);

			m_fbo       = 0;
			m_depth_tex = 0;

			glGenFramebuffers(1, &m_fbo);

			glBindTexture(GL_TEXTURE_2D, m_depth_tex);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_tex, 0);

			for (const auto& id : m_attachments)
			{
				glBindTexture(GL_TEXTURE_2D, id);

				GLint format = 0;
				glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format);

				const GLenum gl_type = (format == GL_RGBA16F) ? GL_FLOAT : GL_UNSIGNED_BYTE;

				glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, GL_RGBA, gl_type, nullptr);
			}

			glBindTexture(GL_TEXTURE_2D, 0);
			create();
		}

		void GeomBuffer::destroy()
		{
			if (m_attachments.size() > 0)
			{
				glDeleteTextures(m_attachments.size(), m_attachments.data());
				m_attachments.clear();
			}

			glDeleteFramebuffers(1, &m_fbo);
			glDeleteTextures(1, &m_depth_tex);

			m_fbo       = 0;
			m_depth_tex = 0;
		}

		void GeomBuffer::prepare() noexcept
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void GeomBuffer::bind() noexcept
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
			glDisable(GL_BLEND);
			glViewport(0, 0, m_width, m_height);

			m_shader.bind();
		}

		void GeomBuffer::unbind() noexcept
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			m_shader.unbind();
		}

		const unsigned int GeomBuffer::get_fbo() const noexcept
		{
			return m_fbo;
		}

		const unsigned int GeomBuffer::get_depth_tex() const noexcept
		{
			return m_depth_tex;
		}

		const std::vector<unsigned int>& GeomBuffer::get_attachments() const noexcept
		{
			return m_attachments;
		}
	} // namespace graphics
} // namespace galaxy