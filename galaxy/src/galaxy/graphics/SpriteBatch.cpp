///
/// SpriteBatch.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/error/Log.hpp"

#include "SpriteBatch.hpp"

namespace galaxy
{
	namespace graphics
	{
		SpriteBatch::SpriteBatch()
			: m_max_bytes {0}
			, m_bytes_used {0}
			, m_index_count {0}
		{
		}

		SpriteBatch::SpriteBatch(SpriteBatch&& sb)
			: m_max_bytes {0}
			, m_bytes_used {0}
			, m_index_count {0}
		{
			this->m_vao         = std::move(sb.m_vao);
			this->m_max_bytes   = sb.m_max_bytes;
			this->m_bytes_used  = sb.m_bytes_used;
			this->m_vertices    = std::move(sb.m_vertices);
			this->m_index_count = sb.m_index_count;
		}

		SpriteBatch& SpriteBatch::operator=(SpriteBatch&& sb)
		{
			if (this != &sb)
			{
				this->m_vao         = std::move(sb.m_vao);
				this->m_max_bytes   = sb.m_max_bytes;
				this->m_bytes_used  = sb.m_bytes_used;
				this->m_vertices    = std::move(sb.m_vertices);
				this->m_index_count = sb.m_index_count;
			}

			return *this;
		}

		SpriteBatch::~SpriteBatch()
		{
			flush();
		}

		void SpriteBatch::init(const int max_quads)
		{
			if (max_quads <= 0)
			{
				GALAXY_LOG(GALAXY_FATAL, "Attempted to create spritebatch with 0 quads.");
			}

			meta::vector<unsigned int> indices;
			indices.reserve(static_cast<std::size_t>(max_quads) * 6);

			auto increment = 0;
			for (auto counter = 0; counter < max_quads; counter++)
			{
				indices.push_back(0 + increment);
				indices.push_back(1 + increment);
				indices.push_back(3 + increment);
				indices.push_back(1 + increment);
				indices.push_back(2 + increment);
				indices.push_back(3 + increment);

				increment += 4;
			}

			m_max_bytes = max_quads * (sizeof(Vertex) * 4);
			m_vao.create(max_quads * 4, indices, StorageFlag::DYNAMIC_DRAW);
			m_vertices.reserve(static_cast<std::size_t>(max_quads) * 4);
		}

		unsigned int SpriteBatch::push(const meta::vector<Vertex>& vertices)
		{
			const auto size_bytes = vertices.size() * sizeof(Vertex);

			if (!((m_bytes_used + size_bytes) > m_max_bytes))
			{
				m_bytes_used += static_cast<unsigned int>(size_bytes);

				for (const auto& vertex : vertices)
				{
					m_vertices.push_back(vertex);
				}

				return static_cast<unsigned int>(m_vertices.size() - 4);
			}
			else
			{
				GALAXY_LOG(GALAXY_FATAL, "Attempted to upload too many quads to a spritebatch.");
				return 0;
			}
		}

		void SpriteBatch::build()
		{
			// Six indicies per quad. Number of quads is total vertexs / 4.
			// The size here is always a multiple of 4 so no "uneven" division takes place.
			if (m_vertices.size() != 0)
			{
				m_index_count = 6 * (static_cast<int>(m_vertices.size()) / 4);
			}

			flush();
			m_vao.sub_buffer(0, m_vertices);
			m_vertices.clear();
		}

		void SpriteBatch::sub_buffer(const unsigned int index, std::span<Vertex> vertices)
		{
			m_vao.sub_buffer(index, vertices);
		}

		void SpriteBatch::flush()
		{
			m_vao.clear();
		}

		int SpriteBatch::count() const
		{
			return m_index_count;
		}

		const VertexArray& SpriteBatch::vao() const
		{
			return m_vao;
		}
	} // namespace graphics
} // namespace galaxy
