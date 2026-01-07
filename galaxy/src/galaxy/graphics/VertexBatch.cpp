///
/// VertexBatch.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "VertexBatch.hpp"

namespace galaxy
{
	VertexBatch::VertexBatch(const int max, const int vertex_count, const std::vector<unsigned int>& indices) noexcept
		: m_vertex_count {0}
		, m_index_count {0}
		, m_count {0}
	{
		m_index_count   = static_cast<int>(indices.size());
		m_vertex_length = max * vertex_count;
		m_index_length  = max * m_index_count;

		m_vertices.resize(m_vertex_length, {});
		m_vao.reserve(m_vertex_length, m_index_length);

		// Calculate fixed indices and push once.
		std::vector<unsigned int> index_data;
		index_data.reserve(m_index_length);

		auto offset = 0;
		for (auto i = 0; i < max; ++i)
		{
			for (auto j = 0; j < m_index_count; ++j)
			{
				index_data.emplace_back(indices[j] + offset);
			}

			// Each set of 6 indices needs to be offset by vertex_count per renderable (max in this case).
			offset += vertex_count;
		}

		m_vao.sub_buffer_indices(0, m_index_length, index_data);
	}

	VertexBatch::VertexBatch(VertexBatch&& vb) noexcept
	{
		this->m_vertex_count  = vb.m_vertex_count;
		this->m_index_count   = vb.m_index_count;
		this->m_count         = vb.m_count;
		this->m_vertex_length = vb.m_vertex_length;
		this->m_index_length  = vb.m_index_length;
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
			this->m_vertices      = std::move(vb.m_vertices);
			this->m_vao           = std::move(vb.m_vao);
		}

		return *this;
	}

	VertexBatch::~VertexBatch() noexcept
	{
	}

	void VertexBatch::prepare() noexcept
	{
		m_count        = 0;
		m_vertex_count = 0;
	}

	void VertexBatch::push(const std::vector<Vertex>& vertices) noexcept
	{
		for (auto i = 0; i < vertices.size(); ++i)
		{
			m_vertices[m_vertex_count + i].m_pos    = vertices[i].m_pos;
			m_vertices[m_vertex_count + i].m_texels = vertices[i].m_texels;
			m_vertices[m_vertex_count + i].m_index  = vertices[i].m_index;
		}

		m_vertex_count += static_cast<int>(vertices.size());
		m_count        += m_index_count; // each set of 6 indices is 1 renderable to draw.
	}

	void VertexBatch::flush() noexcept
	{
		m_vao.sub_buffer_vertices(0, m_vertex_count, m_vertices);
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
