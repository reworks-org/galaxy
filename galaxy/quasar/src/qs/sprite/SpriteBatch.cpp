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
		:VertexData(), m_count(0), m_indexCount(0), m_maxQuads(maxQuads), m_texture(nullptr), m_vertexs(maxQuads * 4)
	{
		m_maxVertices = maxQuads * 4;
		m_maxIndices = maxQuads * 6;
		m_sprites.reserve(maxQuads);
		
		qs::IndexStorage is(m_maxIndices);

		unsigned int increment = 0;
		for (auto counter = 0; counter < maxQuads; counter++)
		{
			is.push_back(0 + increment);
			is.push_back(1 + increment);
			is.push_back(3 + increment);
			is.push_back(1 + increment);
			is.push_back(2 + increment);
			is.push_back(3 + increment);

			increment += 4;
		}

		is.shrink_to_fit();

		m_vertexBuffer.create<qs::SpriteVertex, qs::BufferTypeDynamic>(m_vertexs);
		m_indexBuffer.create<qs::BufferTypeStatic>(is);

		m_layout.add<qs::SpriteVertex, qs::VATypePosition>(2);
		m_layout.add<qs::SpriteVertex, qs::VATypeTexel>(2);
		m_layout.add<qs::SpriteVertex, qs::VATypeOpacity>(1);

		m_vertexArray.create<qs::SpriteVertex>(m_vertexBuffer, m_indexBuffer, m_layout);
	}

	SpriteBatch::~SpriteBatch() noexcept
	{
		m_vertexs.clear();
		clear();
	}

	void SpriteBatch::begin(qs::BaseTexture* texture) noexcept
	{
		clear();
		m_texture = texture;
	}

	void SpriteBatch::draw(qs::BatchedSprite* sprite) noexcept
	{
		m_sprites.push_back(sprite);
	}

	void SpriteBatch::end() noexcept
	{
		if (m_sprites.size() * 4 > m_maxVertices)
		{
			PL_LOG(PL_ERROR, "Too many quads in batch. Max quads: " + std::to_string(m_maxQuads));
		}
		else
		{
			// Only needed for this function scope.
			static glm::vec4 s_result = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			static protostar::Rect<float>* s_region = nullptr;

			sort();

			for (auto* sprite : m_sprites)
			{
				s_result = sprite->getTransformation() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
				s_region = &sprite->getRegion();

				m_vertexs[m_count + 0].m_pos[0] = s_result.x;
				m_vertexs[m_count + 0].m_pos[1] = s_result.y;
				m_vertexs[m_count + 0].m_texels[0] = s_region->m_x;
				m_vertexs[m_count + 0].m_texels[1] = s_region->m_y;
				m_vertexs[m_count + 0].m_opacity = sprite->getOpacity();

				m_vertexs[m_count + 1].m_pos[0] = s_result.x + s_region->m_width;
				m_vertexs[m_count + 1].m_pos[1] = s_result.y;
				m_vertexs[m_count + 1].m_texels[0] = s_region->m_x + s_region->m_width;
				m_vertexs[m_count + 1].m_texels[1] = s_region->m_y;
				m_vertexs[m_count + 1].m_opacity = sprite->getOpacity();

				m_vertexs[m_count + 2].m_pos[0] = s_result.x + s_region->m_width;
				m_vertexs[m_count + 2].m_pos[1] = s_result.y + s_region->m_height;
				m_vertexs[m_count + 2].m_texels[0] = s_region->m_x + s_region->m_width;
				m_vertexs[m_count + 2].m_texels[1] = s_region->m_y + s_region->m_height;
				m_vertexs[m_count + 2].m_opacity = sprite->getOpacity();

				m_vertexs[m_count + 3].m_pos[0] = s_result.x;
				m_vertexs[m_count + 3].m_pos[1] = s_result.y + s_region->m_height;
				m_vertexs[m_count + 3].m_texels[0] = s_region->m_x;
				m_vertexs[m_count + 3].m_texels[1] = s_region->m_y + s_region->m_height;
				m_vertexs[m_count + 3].m_opacity = sprite->getOpacity();

				m_count += 4;
				m_indexCount += 6;
			}

			glNamedBufferSubData(m_vertexBuffer.id(), 0, (m_sprites.size() * 4) * sizeof(qs::SpriteVertex), m_vertexs.data());+
			s_result = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			s_region = nullptr;
		}
	}

	void SpriteBatch::clear() noexcept
	{
		m_sprites.clear();
		m_texture = nullptr;
		m_count = 0;
		m_indexCount = 0;
	}

	void SpriteBatch::bind() noexcept
	{
		m_texture->bind();
		m_vertexArray.bind();
	}

	void SpriteBatch::unbind() noexcept
	{
		m_vertexArray.unbind();
		m_texture->unbind();
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