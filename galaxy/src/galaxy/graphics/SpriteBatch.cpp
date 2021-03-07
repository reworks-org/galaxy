///
/// SpriteBatch.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <execution>

#include "galaxy/error/Log.hpp"

#include "SpriteBatch.hpp"

namespace galaxy
{
	namespace graphics
	{
		SpriteBatch::SpriteBatch(const unsigned int max_quads)
		    : m_offset {0}, m_max_quads {max_quads}, m_max_vertexs {0}, m_max_indexs {0}, m_used_indexs {0}, m_texture {nullptr}
		{
			m_max_vertexs = m_max_quads * 4;
			m_max_indexs  = m_max_quads * 6;

			std::vector<unsigned int> is;
			is.reserve(m_max_indexs);

			unsigned int increment = 0;
			for (unsigned int counter = 0; counter < m_max_quads; counter++)
			{
				is.push_back(0 + increment);
				is.push_back(1 + increment);
				is.push_back(3 + increment);
				is.push_back(1 + increment);
				is.push_back(2 + increment);
				is.push_back(3 + increment);

				increment += 4;
			}

			m_sprites.reserve(m_max_quads);
			m_vertexs.reserve(m_max_vertexs);

			m_vb.create<BatchVertex>(m_vertexs);
			m_ib.create(is);

			m_layout.add<BatchVertex, meta::VAPosition>(2);
			m_layout.add<BatchVertex, meta::VATexel>(2);
			m_layout.add<BatchVertex, meta::VAOpacity>(1);

			m_va.create<BatchVertex>(m_vb, m_ib, m_layout);
		}

		SpriteBatch::SpriteBatch(SpriteBatch&& sb) noexcept
		{
			this->m_va          = std::move(sb.m_va);
			this->m_vb          = std::move(sb.m_vb);
			this->m_ib          = std::move(sb.m_ib);
			this->m_layout      = std::move(sb.m_layout);
			this->m_offset      = sb.m_offset;
			this->m_max_quads   = sb.m_max_quads;
			this->m_max_vertexs = sb.m_max_vertexs;
			this->m_max_indexs  = sb.m_max_indexs;
			this->m_used_indexs = sb.m_used_indexs;
			this->m_texture     = sb.m_texture;
			this->m_sprites     = std::move(sb.m_sprites);
			this->m_vertexs     = std::move(sb.m_vertexs);

			sb.m_texture = nullptr;
		}

		SpriteBatch& SpriteBatch::operator=(SpriteBatch&& sb) noexcept
		{
			if (this != &sb)
			{
				this->m_va          = std::move(sb.m_va);
				this->m_vb          = std::move(sb.m_vb);
				this->m_ib          = std::move(sb.m_ib);
				this->m_layout      = std::move(sb.m_layout);
				this->m_offset      = sb.m_offset;
				this->m_max_quads   = sb.m_max_quads;
				this->m_max_vertexs = sb.m_max_vertexs;
				this->m_max_indexs  = sb.m_max_indexs;
				this->m_used_indexs = sb.m_used_indexs;
				this->m_texture     = sb.m_texture;
				this->m_sprites     = std::move(sb.m_sprites);
				this->m_vertexs     = std::move(sb.m_vertexs);

				sb.m_texture = nullptr;
			}

			return *this;
		}

		SpriteBatch::~SpriteBatch() noexcept
		{
			clear();
		}

		void SpriteBatch::set_texture(BaseTexture* texture) noexcept
		{
			if (!texture)
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to set a nullptr BaseTexture to spritebatch.");
			}
			else
			{
				m_texture = texture;
			}
		}

		void SpriteBatch::add(components::BatchSprite* sprite, components::Transform* transform, const int z_level)
		{
			if (!sprite || !transform)
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to add nullptr to spritebatch.");
			}
			else
			{
				if (((m_sprites.size() + 1) * 4) > m_max_vertexs)
				{
					GALAXY_LOG(GALAXY_ERROR, "Too many quads in batch. Sprite not added.");
				}
				else
				{
					sprite->m_offset  = m_offset;
					sprite->m_z_level = z_level;
					m_offset += 4;
					m_used_indexs += 6;

					m_vertexs.resize(m_offset);
					m_sprites.emplace_back(std::make_pair(sprite, transform));
				}
			}
		}

		void SpriteBatch::calculate()
		{
			sort();

			for (const auto& [sprite, transform] : m_sprites)
			{
				sprite->m_vertexs.clear();

				const auto result1 = transform->get_transform() * glm::vec4 {0.0f, 0.0f, 0.0f, 1.0f};
				sprite->m_vertexs.emplace_back(result1.x, result1.y);

				m_vertexs[sprite->m_offset + 0].m_pos[0]    = result1.x;
				m_vertexs[sprite->m_offset + 0].m_pos[1]    = result1.y;
				m_vertexs[sprite->m_offset + 0].m_texels[0] = sprite->m_region.m_x;
				m_vertexs[sprite->m_offset + 0].m_texels[1] = sprite->m_region.m_y;
				m_vertexs[sprite->m_offset + 0].m_opacity   = sprite->m_opacity;

				const auto result2 = transform->get_transform() * glm::vec4 {0.0f + sprite->m_region.m_width, 0.0f, 0.0f, 1.0f};
				sprite->m_vertexs.emplace_back(result2.x, result2.y);

				m_vertexs[sprite->m_offset + 1].m_pos[0]    = result2.x;
				m_vertexs[sprite->m_offset + 1].m_pos[1]    = result2.y;
				m_vertexs[sprite->m_offset + 1].m_texels[0] = sprite->m_region.m_x + sprite->m_region.m_width;
				m_vertexs[sprite->m_offset + 1].m_texels[1] = sprite->m_region.m_y;
				m_vertexs[sprite->m_offset + 1].m_opacity   = sprite->m_opacity;

				const auto result3 = transform->get_transform() * glm::vec4 {0.0f + sprite->m_region.m_width, 0.0f + sprite->m_region.m_height, 0.0f, 1.0f};
				sprite->m_vertexs.emplace_back(result3.x, result3.y);

				m_vertexs[sprite->m_offset + 2].m_pos[0]    = result3.x;
				m_vertexs[sprite->m_offset + 2].m_pos[1]    = result3.y;
				m_vertexs[sprite->m_offset + 2].m_texels[0] = sprite->m_region.m_x + sprite->m_region.m_width;
				m_vertexs[sprite->m_offset + 2].m_texels[1] = sprite->m_region.m_y + sprite->m_region.m_height;
				m_vertexs[sprite->m_offset + 2].m_opacity   = sprite->m_opacity;

				const auto result4 = transform->get_transform() * glm::vec4 {0.0f, 0.0f + sprite->m_region.m_height, 0.0f, 1.0f};
				sprite->m_vertexs.emplace_back(result4.x, result4.y);

				m_vertexs[sprite->m_offset + 3].m_pos[0]    = result4.x;
				m_vertexs[sprite->m_offset + 3].m_pos[1]    = result4.y;
				m_vertexs[sprite->m_offset + 3].m_texels[0] = sprite->m_region.m_x;
				m_vertexs[sprite->m_offset + 3].m_texels[1] = sprite->m_region.m_y + sprite->m_region.m_height;
				m_vertexs[sprite->m_offset + 3].m_opacity   = sprite->m_opacity;
			}

			glNamedBufferSubData(m_vb.id(), 0, sizeof(BatchVertex) * m_vertexs.size(), m_vertexs.data());
		}

		void SpriteBatch::calculate(components::Transform* global_transform)
		{
			if (!global_transform)
			{
				GALAXY_LOG(GALAXY_FATAL, "SpriteBatch global transform was nullptr.");
			}
			else
			{
				sort();

				for (const auto& [sprite, transform] : m_sprites)
				{
					sprite->m_vertexs.clear();

					const auto result1 = global_transform->get_transform() * transform->get_transform() * glm::vec4 {0.0f, 0.0f, 0.0f, 1.0f};
					sprite->m_vertexs.emplace_back(result1.x, result1.y);

					m_vertexs[sprite->m_offset + 0].m_pos[0]    = result1.x;
					m_vertexs[sprite->m_offset + 0].m_pos[1]    = result1.y;
					m_vertexs[sprite->m_offset + 0].m_texels[0] = sprite->m_region.m_x;
					m_vertexs[sprite->m_offset + 0].m_texels[1] = sprite->m_region.m_y;
					m_vertexs[sprite->m_offset + 0].m_opacity   = sprite->m_opacity;

					const auto result2 = global_transform->get_transform() * transform->get_transform() * glm::vec4 {0.0f + sprite->m_region.m_width, 0.0f, 0.0f, 1.0f};
					sprite->m_vertexs.emplace_back(result2.x, result2.y);

					m_vertexs[sprite->m_offset + 1].m_pos[0]    = result2.x;
					m_vertexs[sprite->m_offset + 1].m_pos[1]    = result2.y;
					m_vertexs[sprite->m_offset + 1].m_texels[0] = sprite->m_region.m_x + sprite->m_region.m_width;
					m_vertexs[sprite->m_offset + 1].m_texels[1] = sprite->m_region.m_y;
					m_vertexs[sprite->m_offset + 1].m_opacity   = sprite->m_opacity;

					const auto result3 = global_transform->get_transform() * transform->get_transform() * glm::vec4 {0.0f + sprite->m_region.m_width, 0.0f + sprite->m_region.m_height, 0.0f, 1.0f};
					sprite->m_vertexs.emplace_back(result3.x, result3.y);

					m_vertexs[sprite->m_offset + 2].m_pos[0]    = result3.x;
					m_vertexs[sprite->m_offset + 2].m_pos[1]    = result3.y;
					m_vertexs[sprite->m_offset + 2].m_texels[0] = sprite->m_region.m_x + sprite->m_region.m_width;
					m_vertexs[sprite->m_offset + 2].m_texels[1] = sprite->m_region.m_y + sprite->m_region.m_height;
					m_vertexs[sprite->m_offset + 2].m_opacity   = sprite->m_opacity;

					const auto result4 = global_transform->get_transform() * transform->get_transform() * glm::vec4 {0.0f, 0.0f + sprite->m_region.m_height, 0.0f, 1.0f};
					sprite->m_vertexs.emplace_back(result4.x, result4.y);

					m_vertexs[sprite->m_offset + 3].m_pos[0]    = result4.x;
					m_vertexs[sprite->m_offset + 3].m_pos[1]    = result4.y;
					m_vertexs[sprite->m_offset + 3].m_texels[0] = sprite->m_region.m_x;
					m_vertexs[sprite->m_offset + 3].m_texels[1] = sprite->m_region.m_y + sprite->m_region.m_height;
					m_vertexs[sprite->m_offset + 3].m_opacity   = sprite->m_opacity;
				}

				glNamedBufferSubData(m_vb.id(), 0, sizeof(BatchVertex) * m_vertexs.size(), m_vertexs.data());
			}
		}

		void SpriteBatch::clear_sprites() noexcept
		{
			m_sprites.clear();
			m_vertexs.clear();
			m_offset      = 0;
			m_used_indexs = 0;
		}

		void SpriteBatch::clear() noexcept
		{
			clear_sprites();
			m_texture = nullptr;
		}

		void SpriteBatch::bind() noexcept
		{
			m_va.bind();
			glBindTexture(GL_TEXTURE_2D, m_texture->gl_texture());
		}

		void SpriteBatch::unbind() noexcept
		{
			m_va.unbind();
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		const unsigned int SpriteBatch::get_used_index_count() const noexcept
		{
			return m_used_indexs;
		}

		IndexBuffer& SpriteBatch::get_ibo() noexcept
		{
			return m_ib;
		}

		VertexBuffer& SpriteBatch::get_vbo() noexcept
		{
			return m_vb;
		}

		VertexArray& SpriteBatch::get_vao() noexcept
		{
			return m_va;
		}

		const unsigned int SpriteBatch::index_count() const noexcept
		{
			return m_ib.count();
		}

		const unsigned int SpriteBatch::get_width() const noexcept
		{
			return m_texture->get_width();
		}

		const unsigned int SpriteBatch::get_height() const noexcept
		{
			return m_texture->get_height();
		}

		const bool SpriteBatch::empty() const noexcept
		{
			return m_sprites.empty();
		}

		void SpriteBatch::sort() noexcept
		{
			std::sort(std::execution::par, m_sprites.begin(), m_sprites.end(), [](const auto& left, const auto& right) {
				return left.first->m_z_level < right.first->m_z_level;
			});
		}
	} // namespace graphics
} // namespace galaxy