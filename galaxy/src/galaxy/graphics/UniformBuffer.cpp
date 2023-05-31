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