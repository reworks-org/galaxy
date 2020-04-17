///
/// Sprite.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <cstdarg>

#include "Sprite.hpp"

///
/// Core namespace.
///
namespace qs
{
	Sprite::Sprite() noexcept
		:Transform(), Texture(), m_activeOffset(-1)
	{
	}

	Sprite::~Sprite() noexcept
	{
		unbind();
		glDeleteTextures(1, &m_id);
	}
	
	void Sprite::create(const qs::BufferType bufferType) noexcept
	{
		auto quad = qs::Vertex::make_quad({ 0.0f, 0.0f, static_cast<float>(m_width), static_cast<float>(m_height) }, { 0.0f, 0.0f, 0.0f, 1.0f }, 0.0f, 0.0f);
		m_vertexBuffer.create({ quad[0], quad[1], quad[2], quad[3] }, bufferType);

		m_indexBuffer.create({ 0, 1, 3, 1, 2, 3 }, bufferType);
		
		m_layout.add<qs::PositionVAType>(2);
		m_layout.add<qs::ColourVAType>(4);
		m_layout.add<qs::TexelVAType>(2);

		m_vertexArray.create(m_vertexBuffer, m_indexBuffer, m_layout);
	}

	void Sprite::create(const qs::BufferType bufferType, const protostar::Rect<float>& texSrc) noexcept
	{
		auto quad = qs::Vertex::make_quad({ 0.0f, 0.0f, texSrc.m_width, texSrc.m_height }, { 0.0f, 0.0f, 0.0f, 1.0f }, texSrc.m_x, texSrc.m_y);
		m_vertexBuffer.create({ quad[0], quad[1], quad[2], quad[3] }, bufferType);
		
		m_indexBuffer.create({ 0, 1, 3, 1, 2, 3 }, bufferType);

		m_layout.add<qs::PositionVAType>(2);
		m_layout.add<qs::ColourVAType>(4);
		m_layout.add<qs::TexelVAType>(2);

		m_vertexArray.create(m_vertexBuffer, m_indexBuffer, m_layout);
	}

	void Sprite::create(const VertexQuadStorage& vertexs) noexcept
	{
		auto quadCount = vertexs.size();

		qs::VertexStorage vs;
		vs.reserve(quadCount * 4);
		vs.clear();
		for (const auto& quad : vertexs)
		{
			for (const auto& vertex : quad)
			{
				vs.push_back(vertex);
			}
		}

		vs.shrink_to_fit();
		m_vertexBuffer.create<qs::DynamicBufferType>(vs);

		std::vector<unsigned int> indexs;
		unsigned int increment = 0;
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
		m_indexBuffer.create<qs::DynamicBufferType>(indexs);

		m_layout.add<qs::PositionVAType>(2);
		m_layout.add<qs::ColourVAType>(4);
		m_layout.add<qs::TexelVAType>(2);

		m_vertexArray.create(m_vertexBuffer, m_indexBuffer, m_layout);
	}

	void Sprite::setActiveQuad(const int offset)
	{
		m_activeOffset = offset;
	}

	void Sprite::bind() noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_id);
		m_vertexArray.bind();
	}

	void Sprite::unbind() noexcept
	{
		m_vertexArray.unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Sprite::recalculate() noexcept
	{
		// Calculate change.
		m_modelMatrix = m_translationMatrix * m_rotateMatrix * m_scaledMatrix;
	}

	qs::VertexArray& Sprite::getVAO() noexcept
	{
		return m_vertexArray;
	}

	const unsigned int Sprite::getCount() const noexcept
	{
		return m_indexBuffer.getCount();
	}

	void Sprite::applyTransform() noexcept
	{
		// Apply change to currently active transform.
		auto* vertexs = &m_vertexBuffer.getVertexs();

		if (m_activeOffset == -1)
		{
			for (auto& vertex : *vertexs)
			{
				auto res = m_modelMatrix * glm::vec4(vertex.m_position[0], vertex.m_position[1], 0.0f, 1.0f);
				vertex.m_position = { res.x, res.y };
			}
		}
		else
		{
			qs::Vertex* vertex = &vertexs->at(m_activeOffset + 0);
			auto res = m_modelMatrix * glm::vec4(vertex->m_position[0], vertex->m_position[1], 0.0f, 1.0f);
			vertex->m_position = { res.x, res.y };

			vertex = &vertexs->at(m_activeOffset + 1);
			res = m_modelMatrix * glm::vec4(vertex->m_position[0], vertex->m_position[1], 0.0f, 1.0f);
			vertex->m_position = { res.x, res.y };

			vertex = &vertexs->at(m_activeOffset + 2);
			res = m_modelMatrix * glm::vec4(vertex->m_position[0], vertex->m_position[1], 0.0f, 1.0f);
			vertex->m_position = { res.x, res.y };

			vertex = &vertexs->at(m_activeOffset + 3);
			res = m_modelMatrix * glm::vec4(vertex->m_position[0], vertex->m_position[1], 0.0f, 1.0f);
			vertex->m_position = { res.x, res.y };
		}

		glNamedBufferSubData(m_vertexBuffer.getID(), 0, vertexs->size() * sizeof(qs::Vertex), vertexs->data());

		// Reset.
		reset();
	}
}