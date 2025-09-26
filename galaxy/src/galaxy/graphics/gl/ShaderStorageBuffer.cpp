///
/// ShaderStorageBuffer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "ShaderStorageBuffer.hpp"

namespace galaxy
{
	ShaderStorageBuffer::ShaderStorageBuffer(const int index)
		: m_id {0}
		, m_index {index}
	{
		glCreateBuffers(1, &m_id);
	}

	ShaderStorageBuffer::ShaderStorageBuffer(ShaderStorageBuffer&& s) noexcept
	{
		this->destroy();

		this->m_id    = s.m_id;
		this->m_index = s.m_index;

		s.m_id = 0;
	}

	ShaderStorageBuffer& ShaderStorageBuffer::operator=(ShaderStorageBuffer&& s) noexcept
	{
		if (this != &s)
		{
			this->destroy();

			this->m_id    = s.m_id;
			this->m_index = s.m_index;

			s.m_id = 0;
		}

		return *this;
	}

	ShaderStorageBuffer::~ShaderStorageBuffer()
	{
		destroy();
	}

	void ShaderStorageBuffer::bind() const
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_index, m_id);
	}

	void ShaderStorageBuffer::unbind() const
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_index, 0);
	}

	void ShaderStorageBuffer::clear() const
	{
		auto size = 0;

		glGetNamedBufferParameteriv(m_id, GL_BUFFER_SIZE, &size);
		glNamedBufferData(m_id, size, nullptr, GL_DYNAMIC_DRAW);
	}

	void ShaderStorageBuffer::destroy()
	{
		if (m_id != 0)
		{
			glDeleteBuffers(1, &m_id);
			m_id = 0;
		}
	}

	unsigned int ShaderStorageBuffer::id() const noexcept
	{
		return m_id;
	}
} // namespace galaxy
