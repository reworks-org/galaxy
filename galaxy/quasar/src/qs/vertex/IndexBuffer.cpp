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
		glGenBuffers(1, &m_id);
	}

	IndexBuffer::~IndexBuffer() noexcept
	{
		glDeleteBuffers(1, &m_id);
	}

	void IndexBuffer::bind() noexcept
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	}

	void IndexBuffer::unbind() noexcept
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	unsigned int IndexBuffer::getCount() const noexcept
	{
		return m_count;
	}
}