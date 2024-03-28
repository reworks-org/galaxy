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
			: m_id {0}
			, m_offset {0}
			, m_count {0}
		{
			glCreateBuffers(1, &m_id);
		}

		VertexBuffer::VertexBuffer(VertexBuffer&& v)
		{
			this->m_id     = v.m_id;
			this->m_offset = v.m_offset;
			this->m_count  = v.m_count;

			v.m_id = 0;
		}

		VertexBuffer& VertexBuffer::operator=(VertexBuffer&& v)
		{
			if (this != &v)
			{
				this->m_id     = v.m_id;
				this->m_offset = v.m_offset;
				this->m_count  = v.m_count;

				v.m_id = 0;
			}

			return *this;
		}

		VertexBuffer::~VertexBuffer()
		{
			if (m_id != 0)
			{
				glDeleteBuffers(1, &m_id);
			}
		}

		void VertexBuffer::buffer(std::span<Vertex> vertices, std::span<unsigned int> indicies)
		{
			const auto ind_len = indicies.size_bytes();
			m_offset           = vertices.size_bytes();
			m_count            = static_cast<int>(indicies.size());

			glNamedBufferData(m_id, ind_len + m_offset, nullptr, GL_DYNAMIC_DRAW);
			glNamedBufferSubData(m_id, m_offset, ind_len, indicies.data());
			glNamedBufferSubData(m_id, 0, m_offset, vertices.data());
		}

		void VertexBuffer::buffer(const int vertex_count, std::span<unsigned int> indicies)
		{
			const auto ind_len = indicies.size_bytes();
			m_offset           = vertex_count * sizeof(Vertex);
			m_count            = static_cast<int>(indicies.size());

			glNamedBufferData(m_id, ind_len + m_offset, nullptr, GL_DYNAMIC_DRAW);
			glNamedBufferSubData(m_id, m_offset, ind_len, indicies.data());
			glNamedBufferSubData(m_id, 0, m_offset, nullptr);
		}

		void VertexBuffer::sub_buffer(const unsigned int index, std::span<Vertex> vertices)
		{
			glNamedBufferSubData(m_id, index * sizeof(Vertex), vertices.size_bytes(), vertices.data());
		}

		void VertexBuffer::clear()
		{
			auto size = 0;

			glGetNamedBufferParameteriv(m_id, GL_BUFFER_SIZE, &size);
			glNamedBufferData(m_id, size, nullptr, GL_DYNAMIC_DRAW);
		}

		int VertexBuffer::count() const
		{
			return m_count;
		}

		void* VertexBuffer::offset()
		{
			return (void*)m_offset;
		}

		unsigned int VertexBuffer::id() const
		{
			return m_id;
		}
	} // namespace graphics
} // namespace galaxy
