///
/// BatchSprite.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include "SpriteBatch.hpp"

///
/// Core namespace.
///
namespace qs
{
	SpriteBatch::SpriteBatch() noexcept
		:Renderable()
	{
	}

	SpriteBatch::~SpriteBatch() noexcept
	{
		m_transformedVertexs.clear();
		m_transforms.clear();
	}

	void SpriteBatch::create(const qs::VertexQuadStorage& vertexs) noexcept
	{
		auto quadCount = vertexs.size();

		qs::VertexStorage vs;
		vs.reserve(quadCount * 4);
		for (const auto& quad : vertexs)
		{
			for (const auto& vertex : quad)
			{
				vs.push_back(vertex);
			}
		}

		vs.shrink_to_fit();
		m_vertexBuffer.create<qs::BufferTypeDynamic>(vs);

		unsigned int increment = 0;
		std::vector<unsigned int> indexs;

		for (auto counter = 0; counter < quadCount; counter++)
		{
			indexs.push_back(0 + increment);
			indexs.push_back(1 + increment);
			indexs.push_back(3 + increment);
			indexs.push_back(1 + increment);
			indexs.push_back(2 + increment);
			indexs.push_back(3 + increment);

			m_transforms.emplace(counter, qs::Transform());

			auto& pos = m_vertexBuffer.getVertexs()[increment + 2].m_position;
			auto& texels = m_vertexBuffer.getVertexs()[increment + 0].m_texels;
			m_transforms[counter].setRotationOrigin(pos[0] * 0.5f, pos[1] * 0.5f); // this does not need to.
			m_transforms[counter].setTexels(texels[0], texels[1]); // this sets dirty flag.
			m_transforms[counter].setOpacity(1.0f);

			increment += 4;
		}

		m_indexBuffer.create<qs::BufferTypeStatic>(indexs);

		m_layout.add<qs::VATypePosition>(2);
		m_layout.add<qs::VATypeColour>(4);
		m_layout.add<qs::VATypeTexel>(2);

		m_vertexArray.create(m_vertexBuffer, m_indexBuffer, m_layout);
		m_transformedVertexs = m_vertexBuffer.getVertexs();
	}

	qs::Transform* SpriteBatch::getTransform(const int offset) noexcept
	{
		if (m_transforms.find(offset) != m_transforms.end())
		{
			return &m_transforms[offset];
		}
		else
		{
			qs::Error::handle().callback("SpriteBatch.cpp", 85, "Failed to find transform at offset: " + std::to_string(offset));
			return nullptr;
		}
	}

	void SpriteBatch::applyTransforms() noexcept
	{
		qs::Vertex* vertex = nullptr;
		qs::Vertex* vertexOrig = nullptr;
		glm::vec4 res = glm::vec4(1.0f);
		glm::vec3* texels = nullptr;
		glm::mat4* curTrnsf = nullptr;
		qs::VertexStorage* orig = &m_vertexBuffer.getVertexs();

		for (auto& tf : m_transforms)
		{
			// This is ok to skip some since the previous values are preserved.
			if (tf.second.isDirty())
			{
				curTrnsf = &tf.second.getTransformation();
				texels = &tf.second.getTexelTransform();

				vertexOrig = &orig->at((tf.first * 4) + 0);
				vertex = &m_transformedVertexs[(tf.first * 4) + 0];
				res = *curTrnsf * glm::vec4(vertexOrig->m_position[0], vertexOrig->m_position[1], 0.0f, 1.0f);
				vertex->m_position[0] = res.x;
				vertex->m_position[1] = res.y;
				vertex->m_colour[3] = texels->z;
				vertex->m_texels[0] = texels->x;
				vertex->m_texels[1] = texels->y;

				vertexOrig = &orig->at((tf.first * 4) + 1);
				vertex = &m_transformedVertexs[(tf.first * 4) + 1];
				res = *curTrnsf * glm::vec4(vertex->m_position[0], vertex->m_position[1], 0.0f, 1.0f);
				vertex->m_position[0] = res.x;
				vertex->m_position[1] = res.y;
				vertex->m_colour[3] = texels->z;
				vertex->m_texels[0] = texels->x + m_width;
				vertex->m_texels[1] = texels->y;

				vertexOrig = &orig->at((tf.first * 4) + 2);
				vertex = &m_transformedVertexs[(tf.first * 4) + 2];
				res = *curTrnsf * glm::vec4(vertex->m_position[0], vertex->m_position[1], 0.0f, 1.0f);
				vertex->m_position[0] = res.x;
				vertex->m_position[1] = res.y;
				vertex->m_colour[3] = texels->z;
				vertex->m_texels[0] = texels->x + m_width;
				vertex->m_texels[1] = texels->y + m_height;

				vertexOrig = &orig->at((tf.first * 4) + 3);
				vertex = &m_transformedVertexs[(tf.first * 4) + 3];
				res = *curTrnsf * glm::vec4(vertex->m_position[0], vertex->m_position[1], 0.0f, 1.0f);
				vertex->m_position[0] = res.x;
				vertex->m_position[1] = res.y;
				vertex->m_colour[3] = texels->z;
				vertex->m_texels[0] = texels->x;
				vertex->m_texels[1] = texels->y + m_height;

				tf.second.setDirty(false);
			}
		}

		glNamedBufferSubData(m_vertexBuffer.getID(), 0, m_transformedVertexs.size() * sizeof(qs::Vertex), m_transformedVertexs.data());
	}
}