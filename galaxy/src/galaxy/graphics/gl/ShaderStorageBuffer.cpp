///
/// ShaderStorageBuffer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "ShaderStorageBuffer.hpp"

namespace galaxy
{
	namespace graphics
	{
		ShaderStorageBuffer::ShaderStorageBuffer(const int index)
			: m_id {0}
			, m_index {index}
		{
			glCreateBuffers(1, &m_id);
		}

		ShaderStorageBuffer::ShaderStorageBuffer(ShaderStorageBuffer&& s)
		{
			if (this->m_id != 0)
			{
				glDeleteBuffers(1, &this->m_id);
			}

			this->m_index = s.m_index;
			this->m_id    = s.m_id;
			s.m_id        = 0;
		}

		ShaderStorageBuffer& ShaderStorageBuffer::operator=(ShaderStorageBuffer&& s)
		{
			if (this != &s)
			{
				if (this->m_id != 0)
				{
					glDeleteBuffers(1, &this->m_id);
				}

				this->m_index = s.m_index;
				this->m_id    = s.m_id;
				s.m_id        = 0;
			}

			return *this;
		}

		ShaderStorageBuffer::~ShaderStorageBuffer()
		{
			if (m_id != 0)
			{
				glDeleteBuffers(1, &m_id);
			}
		}

		void ShaderStorageBuffer::bind() const
		{
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_index, m_id);
		}

		void ShaderStorageBuffer::clear()
		{
			auto size = 0;

			glGetNamedBufferParameteriv(m_id, GL_BUFFER_SIZE, &size);
			glNamedBufferData(m_id, size, nullptr, GL_DYNAMIC_DRAW);
		}

		unsigned int ShaderStorageBuffer::id() const
		{
			return m_id;
		}
	} // namespace graphics
} // namespace galaxy
