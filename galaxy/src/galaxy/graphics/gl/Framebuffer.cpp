///
/// Framebuffer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "galaxy/error/Log.hpp"

#include "Framebuffer.hpp"

namespace galaxy
{
	namespace graphics
	{
		Framebuffer::Framebuffer()
			: m_id {0}
			, m_width {0}
			, m_height {0}
			, m_colour {0}
			, m_renderbuffer {0}
			, m_storageattach {0}
			, m_clear_colour {0.0f, 0.0f, 0.0f, 0.0f}
		{
			glCreateFramebuffers(1, &m_id);
		}

		Framebuffer::Framebuffer(Framebuffer&& f)
		{
			if (this->m_id != 0)
			{
				glDeleteFramebuffers(1, &this->m_id);
			}

			if (this->m_colour != 0)
			{
				glDeleteTextures(1, &this->m_colour);
			}

			if (this->m_renderbuffer != 0)
			{
				glDeleteRenderbuffers(1, &this->m_renderbuffer);
			}

			if (this->m_storageattach != 0)
			{
				glDeleteTextures(1, &this->m_storageattach);
			}

			this->m_id            = f.m_id;
			this->m_width         = f.m_width;
			this->m_height        = f.m_height;
			this->m_colour        = f.m_colour;
			this->m_renderbuffer  = f.m_renderbuffer;
			this->m_storageattach = f.m_storageattach;
			this->m_clear_colour  = std::move(f.m_clear_colour);

			f.m_id            = 0;
			f.m_colour        = 0;
			f.m_renderbuffer  = 0;
			f.m_storageattach = 0;
		}

		Framebuffer& Framebuffer::operator=(Framebuffer&& f)
		{
			if (this != &f)
			{
				if (this->m_id != 0)
				{
					glDeleteFramebuffers(1, &this->m_id);
				}

				if (this->m_colour != 0)
				{
					glDeleteTextures(1, &this->m_colour);
				}

				if (this->m_renderbuffer != 0)
				{
					glDeleteRenderbuffers(1, &this->m_renderbuffer);
				}

				if (this->m_storageattach != 0)
				{
					glDeleteTextures(1, &this->m_storageattach);
				}

				this->m_id            = f.m_id;
				this->m_width         = f.m_width;
				this->m_height        = f.m_height;
				this->m_colour        = f.m_colour;
				this->m_renderbuffer  = f.m_renderbuffer;
				this->m_storageattach = f.m_storageattach;
				this->m_clear_colour  = std::move(f.m_clear_colour);

				f.m_id            = 0;
				f.m_colour        = 0;
				f.m_renderbuffer  = 0;
				f.m_storageattach = 0;
			}

			return *this;
		}

		Framebuffer::~Framebuffer()
		{
			if (m_id != 0)
			{
				glDeleteFramebuffers(1, &m_id);
			}

			if (m_colour != 0)
			{
				glDeleteTextures(1, &m_colour);
			}

			if (m_renderbuffer != 0)
			{
				glDeleteRenderbuffers(1, &m_renderbuffer);
			}

			if (m_storageattach != 0)
			{
				glDeleteTextures(1, &m_storageattach);
			}
		}

		void Framebuffer::add_colour_attachment(const int width, const int height)
		{
			m_width  = width;
			m_height = height;

			if (m_colour != 0)
			{
				glDeleteTextures(1, &m_colour);
			}

			glCreateTextures(GL_TEXTURE_2D, 1, &m_colour);
			glTextureStorage2D(m_colour, 1, GL_RGBA8, m_width, m_height);
			// glTextureSubImage2D(m_colour, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			glTextureParameteri(m_colour, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(m_colour, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTextureParameteri(m_colour, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTextureParameteri(m_colour, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(m_colour, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTextureParameterf(m_colour, GL_TEXTURE_MAX_ANISOTROPY, 1.0f);

			static auto s_fbo = 0;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &s_fbo);

			glBindFramebuffer(GL_FRAMEBUFFER, m_id);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colour, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, s_fbo);
		}

		void Framebuffer::add_depth_stencil_renderbuffer()
		{
			if (m_renderbuffer != 0)
			{
				glDeleteRenderbuffers(1, &m_renderbuffer);
			}

			static auto s_fbo = 0;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &s_fbo);

			glBindFramebuffer(GL_FRAMEBUFFER, m_id);

			glGenRenderbuffers(1, &m_renderbuffer);
			glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderbuffer);

			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, s_fbo);
		}

		void Framebuffer::add_storage_attachment()
		{
			if (m_storageattach != 0)
			{
				glDeleteTextures(1, &m_storageattach);
			}

			glCreateTextures(GL_TEXTURE_2D, 1, &m_storageattach);
			glTextureStorage2D(m_storageattach, 1, GL_R32I, m_width, m_height);
			// glTextureSubImage2D(m_storageattach, 0, 0, 0, m_width, m_height, GL_RED_INTEGER, GL_INT, nullptr);
			glTextureParameteri(m_storageattach, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(m_storageattach, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTextureParameteri(m_storageattach, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTextureParameteri(m_storageattach, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(m_storageattach, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTextureParameterf(m_storageattach, GL_TEXTURE_MAX_ANISOTROPY, 1.0f);

			static auto s_fbo = 0;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &s_fbo);

			glBindFramebuffer(GL_FRAMEBUFFER, m_id);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_storageattach, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, s_fbo);
		}

		void Framebuffer::create()
		{
			meta::vector<unsigned int> used;

			if (m_colour > 0)
			{
				used.push_back(GL_COLOR_ATTACHMENT0);
			}

			if (m_storageattach > 0)
			{
				used.push_back(GL_COLOR_ATTACHMENT1);
			}

			glNamedFramebufferDrawBuffers(m_id, static_cast<GLsizei>(used.size()), used.data());

			const auto status = glCheckNamedFramebufferStatus(m_id, GL_FRAMEBUFFER);
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
		}

		int Framebuffer::read_storagebuffer(const int x, const int y)
		{
			static auto s_fbo = 0;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &s_fbo);

			glBindFramebuffer(GL_FRAMEBUFFER, m_id);
			glReadBuffer(1);

			int data = -1;
			glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &data);

			glBindFramebuffer(GL_FRAMEBUFFER, s_fbo);
			return data;
		}

		void Framebuffer::clear_storagebuffer(const int val)
		{
			glClearNamedFramebufferiv(m_id, GL_COLOR, 1, &val);
		}

		void Framebuffer::set_clear_colour(const graphics::Colour& col)
		{
			const auto& vec = col.vec4();

			m_clear_colour[0] = vec.x;
			m_clear_colour[1] = vec.y;
			m_clear_colour[2] = vec.z;
			m_clear_colour[3] = vec.w;
		}

		void Framebuffer::begin(const bool clear)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_id);
			glViewport(0, 0, m_width, m_height);

			if (clear)
			{
				glClearColor(m_clear_colour[0], m_clear_colour[1], m_clear_colour[2], m_clear_colour[3]);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			}
		}

		void Framebuffer::end()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void Framebuffer::clear()
		{
			if (m_colour != 0)
			{
				clear(0);
			}

			if (m_renderbuffer != 0)
			{
				constexpr const auto CLEAR_DEPTH = 1.0f;
				glClearNamedFramebufferfv(m_renderbuffer, GL_DEPTH, 0, &CLEAR_DEPTH);
			}

			if (m_storageattach > 0)
			{
				clear(1);
			}
		}

		void Framebuffer::clear(const unsigned int index)
		{
			glClearNamedFramebufferfv(m_id, GL_COLOR, index, m_clear_colour.data());
		}

		void Framebuffer::reset()
		{
			glDeleteFramebuffers(1, &m_id);

			if (m_colour != 0)
			{
				glDeleteTextures(1, &m_colour);
			}

			if (m_renderbuffer != 0)
			{
				glDeleteRenderbuffers(1, &m_renderbuffer);
			}

			if (m_storageattach != 0)
			{
				glDeleteTextures(1, &m_storageattach);
			}

			glCreateFramebuffers(1, &m_id);
		}

		int Framebuffer::width() const
		{
			return m_width;
		}

		int Framebuffer::height() const
		{
			return m_height;
		}

		unsigned int Framebuffer::texture() const
		{
			return m_colour;
		}

		unsigned int Framebuffer::id() const
		{
			return m_id;
		}
	} // namespace graphics
} // namespace galaxy
