///
/// VertexBatch.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/error/Log.hpp"

#include "VertexBatch.hpp"

namespace galaxy
{
	namespace graphics
	{
		VertexBatch::VertexBatch()
			: m_max_quads {0}
			, m_index {0}
		{
		}

		VertexBatch::VertexBatch(VertexBatch&& s)
		{
			this->m_vao       = std::move(s.m_vao);
			this->m_max_quads = s.m_max_quads;
			this->m_index     = s.m_index;
		}

		VertexBatch& VertexBatch::operator=(VertexBatch&& s)
		{
			if (this != &s)
			{
				this->m_vao       = std::move(s.m_vao);
				this->m_max_quads = s.m_max_quads;
				this->m_index     = s.m_index;
			}

			return *this;
		}

		VertexBatch::~VertexBatch()
		{
		}

		void VertexBatch::init(const int max_quads)
		{
			m_max_quads = max_quads;

			if (m_max_quads > 0)
			{
				meta::vector<unsigned int> indices;
				indices.reserve(m_max_quads * 6);

				auto increment = 0;
				for (auto counter = 0; counter < m_max_quads; counter++)
				{
					indices.push_back(0 + increment);
					indices.push_back(1 + increment);
					indices.push_back(3 + increment);
					indices.push_back(1 + increment);
					indices.push_back(2 + increment);
					indices.push_back(3 + increment);

					increment += 4;
				}

				m_vao.buffer(m_max_quads * 4, indices);
			}
			else
			{
				GALAXY_LOG(GALAXY_FATAL, "Attempted to create vertexbatch with 0 quads.");
			}
		}

		int VertexBatch::push(std::span<Vertex> vertices)
		{
			if (!(((m_index + vertices.size()) / 4) > m_max_quads))
			{
				m_vao.sub_buffer(m_index, vertices);
				m_index += 4;

				return m_index - 4;
			}
			else
			{
				GALAXY_LOG(GALAXY_FATAL, "Attempted to upload too many quads to a vertexbatch.");
				return -1;
			}
		}

		void VertexBatch::sub_buffer(const unsigned int index, std::span<Vertex> vertices)
		{
			m_vao.sub_buffer(m_index, vertices);
		}

		void VertexBatch::clear()
		{
			m_vao.vbo().clear();
		}

		VertexArray& VertexBatch::vao()
		{
			return m_vao;
		}
	} // namespace graphics
} // namespace galaxy
