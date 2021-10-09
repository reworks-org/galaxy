///
/// IndexBuffer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "IndexBuffer.hpp"

namespace galaxy
{
	namespace graphics
	{
		IndexBuffer::IndexBuffer() noexcept
		    : m_id {0}, m_count {0}
		{
			glGenBuffers(1, &m_id);
		}

		IndexBuffer::IndexBuffer(IndexBuffer&& ib) noexcept
		{
			this->m_id    = ib.m_id;
			this->m_count = ib.m_count;

			ib.m_id    = 0;
			ib.m_count = 0;
		}

		IndexBuffer& IndexBuffer::operator=(IndexBuffer&& ib) noexcept
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

		IndexBuffer::~IndexBuffer() noexcept
		{
			glDeleteBuffers(1, &m_id);
		}

		void IndexBuffer::create(std::span<unsigned int> indices)
		{
			m_count = static_cast<unsigned int>(indices.size());
			bind();

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size_bytes(), indices.data(), GL_DYNAMIC_DRAW);

			unbind();
		}

		void IndexBuffer::bind() noexcept
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		}

		void IndexBuffer::unbind() noexcept
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		const unsigned int IndexBuffer::count() const noexcept
		{
			return m_count;
		}
	} // namespace graphics
} // namespace galaxy