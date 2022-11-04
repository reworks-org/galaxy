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
		SpriteBatch::SpriteBatch() noexcept
			: m_max_bytes {0}
			, m_bytes_used {0}
		{
		}

		SpriteBatch::SpriteBatch(SpriteBatch&& sb) noexcept
			: m_max_bytes {0}
			, m_bytes_used {0}
		{
			this->m_vao        = std::move(sb.m_vao);
			this->m_max_bytes  = sb.m_max_bytes;
			this->m_bytes_used = sb.m_bytes_used;
			this->m_vertices   = std::move(sb.m_vertices);
		}

		SpriteBatch& SpriteBatch::operator=(SpriteBatch&& sb) noexcept
		{
			if (this != &sb)
			{
				this->m_vao        = std::move(sb.m_vao);
				this->m_max_bytes  = sb.m_max_bytes;
				this->m_bytes_used = sb.m_bytes_used;
				this->m_vertices   = std::move(sb.m_vertices);
			}

			return *this;
		}

		SpriteBatch::~SpriteBatch() noexcept
		{
			flush();
		}

		void SpriteBatch::init(const int max_quads)
		{
			std::vector<unsigned int> indices;
			indices.reserve(max_quads * 6);

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
			m_vao.create(m_max_bytes, StorageFlag::DYNAMIC_DRAW, indices, StorageFlag::DYNAMIC_DRAW);
			m_vertices.reserve(max_quads);
		}

		unsigned int SpriteBatch::push(const std::vector<Vertex>& vertices) noexcept
		{
			const auto size_bytes = vertices.size() * sizeof(Vertex);

			if (!((m_bytes_used + size_bytes) > m_max_bytes))
			{
				m_bytes_used += static_cast<unsigned int>(size_bytes);

				for (const auto& vertex : vertices)
				{
					m_vertices.push_back(vertex);
				}

				return m_bytes_used / (4 * sizeof(Vertex));
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to upload too many quads to a spritebatch.");
				return -1;
			}
		}

		void SpriteBatch::commit() noexcept
		{
			m_vao.sub_buffer(0, m_vertices);
			m_vertices.clear();
		}

		void SpriteBatch::sub_buffer(const unsigned int index, std::span<Vertex> vertices) noexcept
		{
			m_vao.sub_buffer(index, vertices);
		}

		void SpriteBatch::flush() noexcept
		{
			m_vao.clear();
		}

		const VertexArray& SpriteBatch::vao() const noexcept
		{
			return m_vao;
		}
	} // namespace graphics
} // namespace galaxy