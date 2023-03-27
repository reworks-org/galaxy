///
/// UniformBuffer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "UniformBuffer.hpp"

namespace galaxy
{
	namespace graphics
	{
		UniformBuffer::UniformBuffer()
			: m_ubo {0}
		{
			glGenBuffers(1, &m_ubo);
		}

		UniformBuffer::UniformBuffer(const unsigned int index)
			: m_ubo {0}
		{
			glGenBuffers(1, &m_ubo);
			create(index);
		}

		UniformBuffer::UniformBuffer(UniformBuffer&& ub)
		{
			if (this->m_ubo != 0)
			{
				glDeleteBuffers(1, &this->m_ubo);
				this->m_ubo = 0;
			}

			this->m_ubo = ub.m_ubo;
			ub.m_ubo    = 0;
		}

		UniformBuffer& UniformBuffer::operator=(UniformBuffer&& ub)
		{
			if (this != &ub)
			{
				if (this->m_ubo != 0)
				{
					glDeleteBuffers(1, &this->m_ubo);
					this->m_ubo = 0;
				}

				this->m_ubo = ub.m_ubo;
				ub.m_ubo    = 0;
			}

			return *this;
		}

		UniformBuffer::~UniformBuffer()
		{
			if (m_ubo != 0)
			{
				glDeleteBuffers(1, &m_ubo);
				m_ubo = 0;
			}
		}

		void UniformBuffer::create(const unsigned int index)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
			glBindBufferBase(GL_UNIFORM_BUFFER, index, m_ubo);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		void UniformBuffer::reserve(const unsigned int size)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);

			GLint64 buffer_size = 0;
			glGetBufferParameteri64v(GL_UNIFORM_BUFFER, GL_BUFFER_SIZE, &buffer_size);

			if (buffer_size != size)
			{
				glInvalidateBufferData(m_ubo);
				glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
			}

			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		void UniformBuffer::bind()
		{
			glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
		}

		void UniformBuffer::unbind()
		{
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		void UniformBuffer::reset()
		{
			glInvalidateBufferData(m_ubo);
			glDeleteBuffers(1, &m_ubo);
			glGenBuffers(1, &m_ubo);
		}

		unsigned int UniformBuffer::id() const
		{
			return m_ubo;
		}
	} // namespace graphics
} // namespace galaxy