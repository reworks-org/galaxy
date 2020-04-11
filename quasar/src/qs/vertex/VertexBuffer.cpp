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

	void VertexBuffer::create(const std::vector<float>& data, unsigned int glDrawType) noexcept
	{
		// Gen a single buffer for this object.
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_id);

		// Copy data into buffer object.
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), data.data(), glDrawType);

		// Clean up.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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