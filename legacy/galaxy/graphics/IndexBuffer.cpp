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
			: m_ibo {0}
			, m_count {0}
		{
			glGenBuffers(1, &m_ibo);
		}

		IndexBuffer::IndexBuffer(IndexBuffer&& ib) noexcept
		{
			this->m_ibo   = ib.m_ibo;
			this->m_count = ib.m_count;

			ib.m_ibo   = 0;
			ib.m_count = 0;
		}

		IndexBuffer& IndexBuffer::operator=(IndexBuffer&& ib) noexcept
		{
			if (this != &ib)
			{
				this->m_ibo   = ib.m_ibo;
				this->m_count = ib.m_count;

				ib.m_ibo   = 0;
				ib.m_count = 0;
			}

			return *this;
		}

		IndexBuffer::~IndexBuffer() noexcept
		{
			destroy();
		}

		void IndexBuffer::create(std::span<unsigned int> indices, const bool single_write)
		{
			m_count = static_cast<unsigned int>(indices.size());

			const auto draw_type = (single_write == true) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size_bytes(), indices.data(), draw_type);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		void IndexBuffer::bind() noexcept
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		}

		void IndexBuffer::unbind() noexcept
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		void IndexBuffer::destroy() noexcept
		{
			glDeleteBuffers(1, &m_ibo);
			m_ibo = 0;
		}

		const int IndexBuffer::index_count() const noexcept
		{
			return m_count;
		}
	} // namespace graphics
} // namespace galaxy