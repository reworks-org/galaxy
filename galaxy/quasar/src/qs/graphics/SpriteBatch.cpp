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
		m_transfVB.clear();
		m_transforms.clear();
	}

	void SpriteBatch::create(const qs::VertexQuadStorage& vertexs) noexcept
	{
		auto quadCount = vertexs.size();
		m_transforms.resize(quadCount, qs::Transform()); // create all transforms.

		qs::VertexStorage vs;
		vs.reserve(quadCount * 4);
		auto quadCounter = 0;
		for (const auto& quad : vertexs)
		{
			for (const auto& vertex : quad)
			{
				vs.push_back(vertex);
			}
			
			m_transforms[quadCounter].setRotationOrigin(quad[2].m_position[0] * 0.5f, quad[2].m_position[1] * 0.5f);
			m_transforms[quadCounter].setTexels(quad[0].m_texels[0], quad[0].m_texels[1]);
			m_transforms[quadCounter].setOpacity(1.0f);
			m_transforms[quadCounter].setDirty(true);

			quadCounter++;
		}

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

			increment += 4;
		}

		m_indexBuffer.create<qs::BufferTypeStatic>(indexs);

		m_layout.add<qs::VATypePosition>(2);
		m_layout.add<qs::VATypeColour>(4);
		m_layout.add<qs::VATypeTexel>(2);

		m_vertexArray.create(m_vertexBuffer, m_indexBuffer, m_layout);
		m_transfVB = m_vertexBuffer.getVertexs();
	}

	qs::Transform* SpriteBatch::getTransform(const int offset) noexcept
	{
		if (offset > m_transforms.size())
		{
			qs::Error::handle().callback("SpriteBatch.cpp", 80, "Invalid offset at: " + std::to_string(offset));
			return nullptr;
		}
		else
		{
			return &m_transforms[offset];
		}
	}

	void SpriteBatch::update() noexcept
	{
		static bool bufferData = false;
		static int counter = 0;
		for (auto& transform : m_transforms)
		{
			if (transform.isDirty())
			{
				auto* origBuf = &m_vertexBuffer.getVertexs();
				auto* texTransf = &transform.getTexelTransform();
				auto cur = counter * 4;
				int curWidth = origBuf->at(cur + 2).m_position[0]; // width of bottom right.
				int curHeight = origBuf->at(cur + 2).m_position[1]; // height of bottom right.

				auto result = transform.getTransformation() * glm::vec4(origBuf->at(cur + 0).m_position[0], origBuf->at(cur + 0).m_position[1], 0.0f, 1.0f);
				m_transfVB[cur + 0].m_position[0] = result.x;
				m_transfVB[cur + 0].m_position[1] = result.y;
				m_transfVB[cur + 0].m_colour[3] = texTransf->z; // opacity
				m_transfVB[cur + 0].m_texels[0] = texTransf->x;
				m_transfVB[cur + 0].m_texels[1] = texTransf->y;

				result = transform.getTransformation() * glm::vec4(origBuf->at(cur + 1).m_position[0], origBuf->at(cur + 1).m_position[1], 0.0f, 1.0f);
				m_transfVB[cur + 1].m_position[0] = result.x;
				m_transfVB[cur + 1].m_position[1] = result.y;
				m_transfVB[cur + 1].m_colour[3] = texTransf->z; // opacity
				m_transfVB[cur + 1].m_texels[0] = texTransf->x + curWidth;
				m_transfVB[cur + 1].m_texels[1] = texTransf->y;

				result = transform.getTransformation() * glm::vec4(origBuf->at(cur + 2).m_position[0], origBuf->at(cur + 2).m_position[1], 0.0f, 1.0f);
				m_transfVB[cur + 2].m_position[0] = result.x;
				m_transfVB[cur + 2].m_position[1] = result.y;
				m_transfVB[cur + 2].m_colour[3] = texTransf->z; // opacity
				m_transfVB[cur + 2].m_texels[0] = texTransf->x + curWidth;
				m_transfVB[cur + 2].m_texels[1] = texTransf->y + curHeight;

				result = transform.getTransformation() * glm::vec4(origBuf->at(cur + 3).m_position[0], origBuf->at(cur + 3).m_position[1], 0.0f, 1.0f);
				m_transfVB[cur + 3].m_position[0] = result.x;
				m_transfVB[cur + 3].m_position[1] = result.y;
				m_transfVB[cur + 3].m_colour[3] = texTransf->z; // opacity
				m_transfVB[cur + 3].m_texels[0] = texTransf->x;
				m_transfVB[cur + 3].m_texels[1] = texTransf->y + curHeight;

				bufferData = true;
				transform.setDirty(false);
				counter++;
			}
		}
		
		// avoids doing this every frame.
		if (bufferData)
		{
			glNamedBufferSubData(m_vertexBuffer.getID(), 0, m_transfVB.size() * sizeof(qs::Vertex), m_transfVB.data());
			bufferData = false;
			counter = 0;
		}
	}
}