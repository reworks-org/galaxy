///
/// IndexBuffer.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include "IndexBuffer.hpp"

///
/// Core namespace.
///
namespace qs
{
	IndexBuffer::IndexBuffer() noexcept
		:m_id(0), m_count(0)
	{
	}

	IndexBuffer::~IndexBuffer() noexcept
	{
		glDeleteBuffers(1, &m_id);
	}

	void IndexBuffer::bind() const noexcept
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	}

	void IndexBuffer::unbind() const noexcept
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	unsigned int IndexBuffer::count() const
	{
		return m_count;
	}
}