///
/// VertexBuffer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "galaxy/logging/Log.hpp"

#include "VertexBuffer.hpp"

namespace galaxy
{
	VertexBuffer::VertexBuffer()
		: m_id {0}
		, m_offset {0}
		, m_count {0}
	{
		glCreateBuffers(1, &m_id);
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& v) noexcept
	{
		if (this->m_id != 0)
		{
			glDeleteBuffers(1, &this->m_id);
		}

		this->m_id     = v.m_id;
		this->m_offset = v.m_offset;
		this->m_count  = v.m_count;

		v.m_id = 0;
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& v) noexcept
	{
		if (this != &v)
		{
			if (this->m_id != 0)
			{
				glDeleteBuffers(1, &this->m_id);
			}

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

	void VertexBuffer::buffer(std::span<Vertex> vertices, std::span<unsigned int> indices)
	{
		const auto ind_offset = indices.size_bytes();
		m_offset              = vertices.size_bytes();
		m_count               = static_cast<int>(indices.size());

		glNamedBufferData(m_id, ind_offset + m_offset, nullptr, GL_DYNAMIC_DRAW);
		glNamedBufferSubData(m_id, 0, m_offset, vertices.data());
		glNamedBufferSubData(m_id, m_offset, ind_offset, indices.data());
	}

	void VertexBuffer::reserve(const int vertex_count, const int index_count)
	{
		const auto vertex_length = vertex_count * sizeof(Vertex);
		const auto index_length  = index_count * sizeof(unsigned int);

		m_offset = vertex_length;
		m_count  = index_count;

		glNamedBufferData(m_id, vertex_length + index_length, nullptr, GL_DYNAMIC_DRAW);
	}

	void
	VertexBuffer::sub_buffer(const unsigned int vi, const int vertex_size, const std::span<Vertex> vertices, unsigned int ei, const int index_size, std::span<unsigned int> indices) const
	{
		glNamedBufferSubData(m_id, vi * sizeof(Vertex), vertex_size * sizeof(Vertex), vertices.data());
		glNamedBufferSubData(m_id, m_offset + (ei * sizeof(unsigned int)), index_size * sizeof(unsigned int), indices.data());
	}

	void VertexBuffer::erase(const unsigned int vi, const int vertex_count, const unsigned int ei, const int index_count) const
	{
		glNamedBufferSubData(m_id, vi * sizeof(Vertex), vertex_count * sizeof(Vertex), nullptr);
		glNamedBufferSubData(m_id, m_offset + (ei * sizeof(unsigned int)), index_count * sizeof(unsigned int), nullptr);
	}

	void VertexBuffer::clear() const
	{
		auto size = 0;

		glGetNamedBufferParameteriv(m_id, GL_BUFFER_SIZE, &size);
		glNamedBufferData(m_id, size, nullptr, GL_DYNAMIC_DRAW);
	}

	int VertexBuffer::count() const noexcept
	{
		return m_count;
	}

	void* VertexBuffer::offset() noexcept
	{
		return (void*)m_offset;
	}

	unsigned int VertexBuffer::id() const noexcept
	{
		return m_id;
	}
} // namespace galaxy
