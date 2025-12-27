///
/// ShaderBuffer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/logging/Log.hpp"

#include "ShaderBuffer.hpp"

namespace galaxy
{
	ShaderBuffer::ShaderBuffer()
		: m_id {0}
		, m_index {-1}
	{
		glCreateBuffers(1, &m_id);
	}

	ShaderBuffer::ShaderBuffer(const GLBufferBinding index)
		: m_id {0}
		, m_index {static_cast<int>(index)}
	{
		glCreateBuffers(1, &m_id);
	}

	ShaderBuffer::ShaderBuffer(ShaderBuffer&& s) noexcept
	{
		this->destroy();

		this->m_id    = s.m_id;
		this->m_index = s.m_index;

		s.m_id = 0;
	}

	ShaderBuffer& ShaderBuffer::operator=(ShaderBuffer&& s) noexcept
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

	ShaderBuffer::~ShaderBuffer()
	{
		destroy();
	}

	void ShaderBuffer::set_index(const GLBufferBinding index)
	{
		m_index = static_cast<int>(index);
	}

	void ShaderBuffer::bind() const
	{
		if (m_index == -1)
		{
			GALAXY_LOG(GALAXY_FATAL, "Tried to bind SSBO with no index.");
		}

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_index, m_id);
	}

	void ShaderBuffer::unbind() const
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_index, 0);
	}

	void ShaderBuffer::clear() const
	{
		auto size = 0;

		glGetNamedBufferParameteriv(m_id, GL_BUFFER_SIZE, &size);
		glNamedBufferData(m_id, size, nullptr, GL_DYNAMIC_DRAW);
	}

	void ShaderBuffer::destroy()
	{
		if (m_id != 0)
		{
			glDeleteBuffers(1, &m_id);
			m_id = 0;
		}
	}

	unsigned int ShaderBuffer::id() const noexcept
	{
		return m_id;
	}
} // namespace galaxy
