///
/// InstanceBuffer.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>
#include <pulsar/Log.hpp>

#include "InstanceBuffer.hpp"

///
/// Core namespace.
///
namespace qs
{
	InstanceBuffer::InstanceBuffer()
	    : m_id {0}, m_divisor {0}
	{
		glGenBuffers(1, &m_id);
	}

	InstanceBuffer::InstanceBuffer(InstanceBuffer&& vb)
	{
		this->m_id      = vb.m_id;
		this->m_divisor = vb.m_divisor;

		vb.m_id = 0;
	}

	InstanceBuffer& InstanceBuffer::operator=(InstanceBuffer&& vb)
	{
		if (this != &vb)
		{
			this->m_id      = vb.m_id;
			this->m_divisor = vb.m_divisor;

			vb.m_id = 0;
		}

		return *this;
	}

	void InstanceBuffer::create(std::span<glm::vec2> offsets, unsigned int divisor)
	{
		if (!offsets.empty())
		{
			m_divisor = divisor;

			bind();

			glBufferData(GL_ARRAY_BUFFER, offsets.size_bytes(), &offsets[0], GL_STATIC_DRAW);

			unbind();
		}
		else
		{
			PL_LOG(PL_WARNING, "Passed empty offset array to instance buffer.");
		}
	}

	InstanceBuffer::~InstanceBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}

	void InstanceBuffer::bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}

	void InstanceBuffer::unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	const unsigned int InstanceBuffer::id() const
	{
		return m_id;
	}

	const unsigned int InstanceBuffer::divisor() const
	{
		return m_divisor;
	}
} // namespace qs