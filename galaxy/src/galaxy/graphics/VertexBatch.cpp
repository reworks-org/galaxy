///
/// VertexBatch.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "VertexBatch.hpp"

namespace galaxy
{
	VertexBatch::VertexBatch(const int max, const int vertex_count, const int index_count) noexcept
		: m_vertex_count {0}
		, m_index_count {0}
		, m_count {0}
	{
		m_vertex_length = max * vertex_count;
		m_index_length  = max * index_count;

		m_vertices.resize(m_vertex_length, {});
		m_indices.resize(m_index_length, 0);

		m_vao.reserve(m_vertex_length, m_index_length);
	}

	VertexBatch::VertexBatch(VertexBatch&& vb) noexcept
	{
		this->m_vertex_count  = vb.m_vertex_count;
		this->m_index_count   = vb.m_index_count;
		this->m_count         = vb.m_count;
		this->m_vertex_length = vb.m_vertex_length;
		this->m_index_length  = vb.m_index_length;
		this->m_indices       = std::move(vb.m_indices);
		this->m_vertices      = std::move(vb.m_vertices);
		this->m_vao           = std::move(vb.m_vao);
	}

	VertexBatch& VertexBatch::operator=(VertexBatch&& vb) noexcept
	{
		if (this != &vb)
		{
			this->m_vertex_count  = vb.m_vertex_count;
			this->m_index_count   = vb.m_index_count;
			this->m_count         = vb.m_count;
			this->m_vertex_length = vb.m_vertex_length;
			this->m_index_length  = vb.m_index_length;
			this->m_indices       = std::move(vb.m_indices);
			this->m_vertices      = std::move(vb.m_vertices);
			this->m_vao           = std::move(vb.m_vao);
		}

		return *this;
	}

	VertexBatch::~VertexBatch() noexcept
	{
	}

	void VertexBatch::push(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) noexcept
	{
		for (auto i = 0; i < vertices.size(); ++i)
		{
			m_vertices[m_vertex_count + i].m_pos    = vertices[i].m_pos;
			m_vertices[m_vertex_count + i].m_texels = vertices[i].m_texels;
			m_vertices[m_vertex_count + i].m_handle = vertices[i].m_handle;
		}

		for (auto i = 0; i < indices.size(); ++i)
		{
			m_indices[m_index_count + i] = indices[i] + m_vertex_count;
		}

		m_vertex_count += static_cast<int>(vertices.size());
		m_index_count  += static_cast<int>(indices.size());
	}

	void VertexBatch::flush() noexcept
	{
		m_vao.sub_buffer(0, m_vertex_count, m_vertices, 0, m_index_count, m_indices);
		m_vao.erase(m_vertex_count, m_vertex_length - m_vertex_count, m_index_count, m_index_length - m_index_count);

		m_count = m_index_count;

		m_vertex_count = 0;
		m_index_count  = 0;
	}

	void VertexBatch::bind() const noexcept
	{
		m_vao.bind();
	}

	int VertexBatch::count() const noexcept
	{
		return m_count;
	}

	void* VertexBatch::offset() noexcept
	{
		return m_vao.offset();
	}

	VertexArray& VertexBatch::vao() noexcept
	{
		return m_vao;
	}
} // namespace galaxy
