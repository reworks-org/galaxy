///
/// GPUBuffer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "GPUBuffer.hpp"

namespace galaxy
{
	GPUBuffer::GPUBuffer(const GLBufferBinding index, const int type)
		: m_id {0}
		, m_index {static_cast<int>(index)}
		, m_type {type}
	{
		glCreateBuffers(1, &m_id);
	}

	GPUBuffer::GPUBuffer(GPUBuffer&& s) noexcept
	{
		this->destroy();

		this->m_id    = s.m_id;
		this->m_index = s.m_index;
		this->m_type  = s.m_type;

		s.m_id = 0;
	}

	GPUBuffer& GPUBuffer::operator=(GPUBuffer&& s) noexcept
	{
		if (this != &s)
		{
			this->destroy();

			this->m_id    = s.m_id;
			this->m_index = s.m_index;
			this->m_type  = s.m_type;

			s.m_id = 0;
		}

		return *this;
	}

	GPUBuffer::~GPUBuffer()
	{
		destroy();
	}

	void GPUBuffer::bind() const
	{
		glBindBufferBase(m_type, m_index, m_id);
	}

	void GPUBuffer::unbind() const
	{
		glBindBufferBase(m_type, m_index, 0);
	}

	void GPUBuffer::clear() const
	{
		auto size = 0;

		glGetNamedBufferParameteriv(m_id, GL_BUFFER_SIZE, &size);
		glNamedBufferData(m_id, size, nullptr, GL_DYNAMIC_DRAW);
	}

	void GPUBuffer::destroy()
	{
		if (m_id != 0)
		{
			glDeleteBuffers(1, &m_id);
			m_id = 0;
		}
	}

	unsigned int GPUBuffer::id() const noexcept
	{
		return m_id;
	}
} // namespace galaxy
