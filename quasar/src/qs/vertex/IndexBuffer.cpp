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

	void IndexBuffer::create(const std::vector<unsigned int>& indexs, const qs::BufferType bufferType) noexcept
	{
		m_count = static_cast<unsigned int>(indexs.size());

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);

		switch (bufferType)
		{
		case qs::BufferType::STATIC:
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(unsigned int), indexs.data(), GL_STATIC_DRAW);
			break;

		case qs::BufferType::DYNAMIC:
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(unsigned int), indexs.data(), GL_DYNAMIC_DRAW);
			break;
		}
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

	unsigned int IndexBuffer::getCount() const
	{
		return m_count;
	}
}