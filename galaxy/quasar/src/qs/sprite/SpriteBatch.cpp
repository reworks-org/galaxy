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
	SpriteBatch::SpriteBatch(const unsigned int maxQuads) noexcept
		:VertexData(), m_offset(0), m_maxQuads(maxQuads), m_maxVertexs(0), m_maxIndexs(0), m_indexCount(0), m_texture(nullptr)
	{
		m_maxVertexs = m_maxQuads * 4;
		m_maxIndexs = m_maxQuads * 6;

		qs::IndexStorage is;
		is.reserve(m_maxIndexs);

		unsigned int increment = 0;
		for (auto counter = 0; counter < m_maxQuads; counter++)
		{
			is.push_back(0 + increment);
			is.push_back(1 + increment);
			is.push_back(3 + increment);
			is.push_back(1 + increment);
			is.push_back(2 + increment);
			is.push_back(3 + increment);

			increment += 4;
		}

		m_sprites.reserve(m_maxQuads);
		m_vertexs.reserve(m_maxVertexs);
		m_vertexBuffer.create<qs::SpriteVertex, qs::BufferTypeDynamic>(m_vertexs, false);
		m_indexBuffer.create<qs::BufferTypeStatic>(is);

		m_layout.add<qs::SpriteVertex, qs::VATypePosition>(2);
		m_layout.add<qs::SpriteVertex, qs::VATypeTexel>(2);
		m_layout.add<qs::SpriteVertex, qs::VATypeOpacity>(1);

		m_vertexArray.create<qs::SpriteVertex>(m_vertexBuffer, m_indexBuffer, m_layout);
	}

	SpriteBatch::~SpriteBatch() noexcept
	{
		clear();
	}

	void SpriteBatch::setTexture(qs::BaseTexture* texture) noexcept
	{
		m_texture = texture;
	}

	void SpriteBatch::add(qs::BatchedSprite* sprite) noexcept
	{
		if (((m_sprites.size() + 1) * 4) > m_maxVertexs)
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
			m_indexCount += 6;

			m_sprites.push_back(sprite);
			sort();
		}
	}

	void SpriteBatch::update() noexcept
	{
		// Only needed for this function scope.
		static bool s_updateRenderData = false;

		for (auto* sprite : m_sprites)
		{
			if (sprite->m_isDirty)
			{
				s_updateRenderData = true;
				sprite->m_isDirty = false;
				glm::vec4 result = sprite->getTransformation() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

				m_vertexs[sprite->m_offset + 0].m_pos[0] = result.x;
				m_vertexs[sprite->m_offset + 0].m_pos[1] = result.y;
				m_vertexs[sprite->m_offset + 0].m_texels[0] = sprite->m_region.m_x;
				m_vertexs[sprite->m_offset + 0].m_texels[1] = sprite->m_region.m_y;
				m_vertexs[sprite->m_offset + 0].m_opacity = sprite->m_opacity;

				m_vertexs[sprite->m_offset + 1].m_pos[0] = result.x + sprite->m_region.m_width;
				m_vertexs[sprite->m_offset + 1].m_pos[1] = result.y;
				m_vertexs[sprite->m_offset + 1].m_texels[0] = sprite->m_region.m_x + sprite->m_region.m_width;
				m_vertexs[sprite->m_offset + 1].m_texels[1] = sprite->m_region.m_y;
				m_vertexs[sprite->m_offset + 1].m_opacity = sprite->m_opacity;

				m_vertexs[sprite->m_offset + 2].m_pos[0] = result.x + sprite->m_region.m_width;
				m_vertexs[sprite->m_offset + 2].m_pos[1] = result.y + sprite->m_region.m_height;
				m_vertexs[sprite->m_offset + 2].m_texels[0] = sprite->m_region.m_x + sprite->m_region.m_width;
				m_vertexs[sprite->m_offset + 2].m_texels[1] = sprite->m_region.m_y + sprite->m_region.m_height;
				m_vertexs[sprite->m_offset + 2].m_opacity = sprite->m_opacity;

				m_vertexs[sprite->m_offset + 3].m_pos[0] = result.x;
				m_vertexs[sprite->m_offset + 3].m_pos[1] = result.y + sprite->m_region.m_height;
				m_vertexs[sprite->m_offset + 3].m_texels[0] = sprite->m_region.m_x;
				m_vertexs[sprite->m_offset + 3].m_texels[1] = sprite->m_region.m_y + sprite->m_region.m_height;
				m_vertexs[sprite->m_offset + 3].m_opacity = sprite->m_opacity;
			}
		}

		if (s_updateRenderData)
		{
			glNamedBufferSubData(m_vertexBuffer.id(), 0, sizeof(qs::SpriteVertex) * m_vertexs.size(), m_vertexs.data());
			s_updateRenderData = false;
		}
	}

	void SpriteBatch::clear() noexcept
	{
		m_texture = nullptr;
		m_sprites.clear();
		m_vertexs.clear();
		m_offset = 0;
		m_indexCount = 0;
	}

	void SpriteBatch::bind() noexcept
	{
		m_vertexArray.bind();
		glBindTexture(GL_TEXTURE_2D, m_texture->getGLTexture());
	}

	void SpriteBatch::unbind() noexcept
	{
		m_vertexArray.unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	const unsigned int SpriteBatch::getCount() const noexcept
	{
		return m_indexCount;
	}

	const unsigned int SpriteBatch::getWidth() const noexcept
	{
		return m_texture->getWidth();
	}

	const unsigned int SpriteBatch::getHeight() const noexcept
	{
		return m_texture->getHeight();
	}

	void SpriteBatch::sort() noexcept
	{
		std::stable_sort(m_sprites.begin(), m_sprites.end(), [](const qs::BatchedSprite* lhs, const qs::BatchedSprite* rhs)
		{
			return lhs->getZLevel() < rhs->getZLevel();
		});
	}
}