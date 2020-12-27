///
/// IndexBuffer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "IndexBuffer.hpp"

namespace galaxy
{
	namespace graphics
	{
		IndexBuffer::IndexBuffer()
		    : m_id {0}, m_count {0}
		{
			glGenBuffers(1, &m_id);
		}

		IndexBuffer::IndexBuffer(IndexBuffer&& ib)
		{
			this->m_id    = ib.m_id;
			this->m_count = ib.m_count;

			ib.m_id    = 0;
			ib.m_count = 0;
		}

		IndexBuffer& IndexBuffer::operator=(IndexBuffer&& ib)
		{
			if (this != &ib)
			{
				this->m_id    = ib.m_id;
				this->m_count = ib.m_count;

				ib.m_id    = 0;
				ib.m_count = 0;
			}

			return *this;
		}

		IndexBuffer::~IndexBuffer()
		{
			glDeleteBuffers(1, &m_id);
		}

		void IndexBuffer::bind()
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		}

		void IndexBuffer::unbind()
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		unsigned int IndexBuffer::count() const
		{
			return m_count;
		}
	} // namespace graphics
} // namespace galaxy