///
/// VertexBuffer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "VertexBuffer.hpp"

namespace galaxy
{
	namespace graphics
	{
		VertexBuffer::VertexBuffer()
		    : m_id {0}, m_size {0}
		{
			glGenBuffers(1, &m_id);
		}

		VertexBuffer::VertexBuffer(VertexBuffer&& vb)
		{
			this->m_id   = vb.m_id;
			this->m_size = vb.m_size;

			vb.m_id   = 0;
			vb.m_size = 0;
		}

		VertexBuffer& VertexBuffer::operator=(VertexBuffer&& vb)
		{
			if (this != &vb)
			{
				this->m_id   = vb.m_id;
				this->m_size = vb.m_size;

				vb.m_id   = 0;
				vb.m_size = 0;
			}

			return *this;
		}

		VertexBuffer::~VertexBuffer()
		{
			glDeleteBuffers(1, &m_id);
		}

		void VertexBuffer::bind()
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_id);
		}

		void VertexBuffer::unbind()
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		const unsigned int VertexBuffer::id() const
		{
			return m_id;
		}
	} // namespace graphics
} // namespace galaxy