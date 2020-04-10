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

	void IndexBuffer::createFromDynamic(const std::vector<unsigned int>& indices, unsigned int glDrawType) noexcept
	{
		m_count = indices.size();

		// Gen a single buffer for this object.
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);

		// Copy data into buffer object.
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count, indices.data(), glDrawType);

		// Clean up.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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