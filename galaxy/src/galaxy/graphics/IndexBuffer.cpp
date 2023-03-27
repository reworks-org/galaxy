///
/// IndexBuffer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/error/Log.hpp"

#include "IndexBuffer.hpp"

namespace galaxy
{
	namespace graphics
	{
		IndexBuffer::IndexBuffer()
			: m_ibo {0}
			, m_count {0}
		{
			glCreateBuffers(1, &m_ibo);
		}

		IndexBuffer::IndexBuffer(const unsigned int capacity, const StorageFlag flag)
			: m_ibo {0}
			, m_count {capacity}
		{
			glCreateBuffers(1, &m_ibo);
			glNamedBufferData(m_ibo, m_count * sizeof(unsigned int), nullptr, static_cast<GLenum>(flag));
		}

		IndexBuffer::IndexBuffer(IndexBuffer&& ib)
			: m_ibo {0}
		{
			this->destroy();

			this->m_ibo   = ib.m_ibo;
			this->m_count = ib.m_count;

			ib.m_ibo = 0;
		}

		IndexBuffer& IndexBuffer::operator=(IndexBuffer&& ib)
		{
			if (this != &ib)
			{
				this->destroy();

				this->m_ibo   = ib.m_ibo;
				this->m_count = ib.m_count;

				ib.m_ibo = 0;
			}

			return *this;
		}

		IndexBuffer::~IndexBuffer()
		{
			destroy();
		}

		void IndexBuffer::create(std::span<unsigned int> indices, const StorageFlag flag)
		{
			if (!indices.empty())
			{
				m_count = static_cast<unsigned int>(indices.size());
				glNamedBufferData(m_ibo, indices.size_bytes(), indices.data(), static_cast<GLenum>(flag));
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to upload empty index buffer.");
			}
		}

		void IndexBuffer::clear()
		{
			glInvalidateBufferData(m_ibo);
		}

		void IndexBuffer::destroy()
		{
			if (m_ibo != 0)
			{
				glDeleteBuffers(1, &m_ibo);
				m_ibo = 0;
			}
		}

		int IndexBuffer::index_count() const
		{
			return m_count;
		}

		unsigned int IndexBuffer::id() const
		{
			return m_ibo;
		}
	} // namespace graphics
} // namespace galaxy