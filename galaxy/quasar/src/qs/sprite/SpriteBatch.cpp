///
/// SpriteBatch.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include <pulsar/Log.hpp>

#include "SpriteBatch.hpp"

///
/// Core namespace.
///
namespace qs
{
	SpriteBatch::SpriteBatch(const unsigned int max_quads)
	    : VertexData {}, m_update_renderdata {false}, m_offset {0}, m_max_quads {max_quads}, m_max_vertexs {0}, m_max_indexs {0}, m_used_indexs {0}, m_texture {nullptr}
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

		m_vb.create<qs::SpriteVertex, qs::BufferDynamic>(m_vertexs);
		m_ib.create<qs::BufferStatic>(is);

		m_layout.add<qs::SpriteVertex, qs::VAPosition>(2);
		m_layout.add<qs::SpriteVertex, qs::VATexel>(2);

		m_va.create<qs::SpriteVertex>(m_vb, m_ib, m_layout);
	}

	SpriteBatch::~SpriteBatch() noexcept
	{
		clear();
	}

	void SpriteBatch::set_texture(qs::BaseTexture* texture) noexcept
	{
		if (texture == nullptr)
		{
			PL_LOG(PL_WARNING, "Attempted to set a nullptr BaseTexture to spritebatch.");
		}
		else
		{
			m_texture = texture;
		}
	}

	void SpriteBatch::add(qs::BatchedSprite* sprite)
	{
		if (sprite == nullptr)
		{
			PL_LOG(PL_WARNING, "Attempted to add nullptr to spritebatch.");
		}
		else
		{
			if (((m_sprites.size() + 1) * 4) > m_max_vertexs)
			{
				PL_LOG(PL_ERROR, "Too many quads in batch. Sprite not added.");
			}
			else
			{
				if ((m_offset + 4) > m_vertexs.size())
				{
					m_vertexs.resize(m_offset + 4);
				}

				sprite->m_offset = m_offset;
				m_offset += 4;
				m_used_indexs += 6;

				m_sprites.push_back(sprite);
				sort();
			}
		}
	}

	void SpriteBatch::update()
	{
		for (auto* sprite : m_sprites)
		{
			if (sprite->m_dirty)
			{
				glm::vec4 result = sprite->get_transform() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

				m_vertexs[sprite->m_offset + 0].m_pos[0]    = result.x;
				m_vertexs[sprite->m_offset + 0].m_pos[1]    = result.y;
				m_vertexs[sprite->m_offset + 0].m_texels[0] = sprite->m_region.m_x;
				m_vertexs[sprite->m_offset + 0].m_texels[1] = sprite->m_region.m_y;

				m_vertexs[sprite->m_offset + 1].m_pos[0]    = result.x + sprite->m_region.m_width;
				m_vertexs[sprite->m_offset + 1].m_pos[1]    = result.y;
				m_vertexs[sprite->m_offset + 1].m_texels[0] = sprite->m_region.m_x + sprite->m_region.m_width;
				m_vertexs[sprite->m_offset + 1].m_texels[1] = sprite->m_region.m_y;

				m_vertexs[sprite->m_offset + 2].m_pos[0]    = result.x + sprite->m_region.m_width;
				m_vertexs[sprite->m_offset + 2].m_pos[1]    = result.y + sprite->m_region.m_height;
				m_vertexs[sprite->m_offset + 2].m_texels[0] = sprite->m_region.m_x + sprite->m_region.m_width;
				m_vertexs[sprite->m_offset + 2].m_texels[1] = sprite->m_region.m_y + sprite->m_region.m_height;

				m_vertexs[sprite->m_offset + 3].m_pos[0]    = result.x;
				m_vertexs[sprite->m_offset + 3].m_pos[1]    = result.y + sprite->m_region.m_height;
				m_vertexs[sprite->m_offset + 3].m_texels[0] = sprite->m_region.m_x;
				m_vertexs[sprite->m_offset + 3].m_texels[1] = sprite->m_region.m_y + sprite->m_region.m_height;

				m_update_renderdata = true;
				sprite->m_dirty     = false;
			}
		}

		if (m_update_renderdata)
		{
			glNamedBufferSubData(m_vb.id(), 0, sizeof(qs::SpriteVertex) * m_vertexs.size(), m_vertexs.data());
			m_update_renderdata = false;
		}
	}

	void SpriteBatch::update(qs::Transform* transform)
	{
		if (!transform)
		{
			PL_LOG(PL_FATAL, "Transform was nullptr.");
		} // Throws exception.

		const bool is_dirty = transform->is_dirty(); // Have to cache here because get_transform() sets is_dirty to false.
		for (auto* sprite : m_sprites)
		{
			if (sprite->m_dirty || is_dirty)
			{
				glm::vec4 result = transform->get_transform() * sprite->get_transform() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

				m_vertexs[sprite->m_offset + 0].m_pos[0]    = result.x;
				m_vertexs[sprite->m_offset + 0].m_pos[1]    = result.y;
				m_vertexs[sprite->m_offset + 0].m_texels[0] = sprite->m_region.m_x;
				m_vertexs[sprite->m_offset + 0].m_texels[1] = sprite->m_region.m_y;

				m_vertexs[sprite->m_offset + 1].m_pos[0]    = result.x + sprite->m_region.m_width;
				m_vertexs[sprite->m_offset + 1].m_pos[1]    = result.y;
				m_vertexs[sprite->m_offset + 1].m_texels[0] = sprite->m_region.m_x + sprite->m_region.m_width;
				m_vertexs[sprite->m_offset + 1].m_texels[1] = sprite->m_region.m_y;

				m_vertexs[sprite->m_offset + 2].m_pos[0]    = result.x + sprite->m_region.m_width;
				m_vertexs[sprite->m_offset + 2].m_pos[1]    = result.y + sprite->m_region.m_height;
				m_vertexs[sprite->m_offset + 2].m_texels[0] = sprite->m_region.m_x + sprite->m_region.m_width;
				m_vertexs[sprite->m_offset + 2].m_texels[1] = sprite->m_region.m_y + sprite->m_region.m_height;

				m_vertexs[sprite->m_offset + 3].m_pos[0]    = result.x;
				m_vertexs[sprite->m_offset + 3].m_pos[1]    = result.y + sprite->m_region.m_height;
				m_vertexs[sprite->m_offset + 3].m_texels[0] = sprite->m_region.m_x;
				m_vertexs[sprite->m_offset + 3].m_texels[1] = sprite->m_region.m_y + sprite->m_region.m_height;

				m_update_renderdata = true;
				sprite->m_dirty     = false;
			}
		}

		if (m_update_renderdata)
		{
			glNamedBufferSubData(m_vb.id(), 0, sizeof(qs::SpriteVertex) * m_vertexs.size(), m_vertexs.data());
			m_update_renderdata = false;
		}
	}

	void SpriteBatch::clear() noexcept
	{
		m_texture = nullptr;
		m_sprites.clear();
		m_vertexs.clear();
		m_offset      = 0;
		m_used_indexs = 0;
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

	const unsigned int SpriteBatch::get_width() const noexcept
	{
		return m_texture->get_width();
	}

	const unsigned int SpriteBatch::get_height() const noexcept
	{
		return m_texture->get_height();
	}

	void SpriteBatch::sort() noexcept
	{
		std::stable_sort(m_sprites.begin(), m_sprites.end(), [](const qs::BatchedSprite* lhs, const qs::BatchedSprite* rhs) noexcept {
			return lhs->z_level() < rhs->z_level();
		});
	}
} // namespace qs