///
/// VertexBuffer.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include "VertexBuffer.hpp"

///
/// Core namespace.
///
namespace qs
{
	VertexBuffer::VertexBuffer() noexcept
		:m_id(0)
	{
	}

	VertexBuffer::~VertexBuffer() noexcept
	{
		glDeleteBuffers(1, &m_id);
	}

	void VertexBuffer::bind() const noexcept
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}

	void VertexBuffer::unbind() const noexcept
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}