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
		:Transform(), Texture()
	{
	}

	Sprite::~Sprite() noexcept
	{
		unbind();
		glDeleteTextures(1, &m_id);

		m_transformedVertexs.clear();
	}
	
	void Sprite::create(const qs::BufferType bufferType) noexcept
	{
		auto quad = qs::Vertex::make_quad({ 0.0f, 0.0f, static_cast<float>(m_width), static_cast<float>(m_height) }, { 0.0f, 0.0f, 0.0f, 1.0f }, 0.0f, 0.0f);
		m_vertexBuffer.create({ quad[0], quad[1], quad[2], quad[3] }, bufferType);

		m_indexBuffer.create({ 0, 1, 3, 1, 2, 3 }, bufferType);
		
		m_layout.add(2, qs::VertexAttribute::Type::POSITION);
		m_layout.add(4, qs::VertexAttribute::Type::COLOUR);
		m_layout.add(2, qs::VertexAttribute::Type::TEXELS);

		m_vertexArray.create(m_vertexBuffer, m_indexBuffer, m_layout);

		setRotationOrigin(static_cast<float>(m_width) * 0.5f, static_cast<float>(m_height) * 0.5f);
	}

	void Sprite::create(const qs::BufferType bufferType, const protostar::Rect<float>& texSrc) noexcept
	{
		auto quad = qs::Vertex::make_quad({ 0.0f, 0.0f, texSrc.m_width, texSrc.m_height }, { 0.0f, 0.0f, 0.0f, 1.0f }, texSrc.m_x, texSrc.m_y);
		m_vertexBuffer.create({ quad[0], quad[1], quad[2], quad[3] }, bufferType);
		
		m_indexBuffer.create({ 0, 1, 3, 1, 2, 3 }, bufferType);

		m_layout.add(2, qs::VertexAttribute::Type::POSITION);
		m_layout.add(4, qs::VertexAttribute::Type::COLOUR);
		m_layout.add(2, qs::VertexAttribute::Type::TEXELS);

		m_vertexArray.create(m_vertexBuffer, m_indexBuffer, m_layout);

		setRotationOrigin(static_cast<float>(m_width) * 0.5f, static_cast<float>(m_height) * 0.5f);
	}

	void Sprite::create(const qs::BufferType bufferType, const VertexQuadStorage& vertexs) noexcept
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
		m_vertexBuffer.create(vs, bufferType);

		std::vector<unsigned int> indexs;
		for (auto counter = 0; counter < quadCount; counter += 4)
		{
			indexs.push_back(0 + counter);
			indexs.push_back(1 + counter);
			indexs.push_back(3 + counter);
			indexs.push_back(1 + counter);
			indexs.push_back(2 + counter);
			indexs.push_back(3 + counter);
		}
		m_indexBuffer.create(indexs, bufferType);

		m_layout.add(2, qs::VertexAttribute::Type::POSITION);
		m_layout.add(4, qs::VertexAttribute::Type::COLOUR);
		m_layout.add(2, qs::VertexAttribute::Type::TEXELS);

		m_vertexArray.create(m_vertexBuffer, m_indexBuffer, m_layout);

		setRotationOrigin(static_cast<float>(m_width) * 0.5f, static_cast<float>(m_height) * 0.5f);
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
		m_modelMatrix = m_translationMatrix * m_rotateMatrix * m_scaledMatrix;
		applyTransform();
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
		if (m_vertexBuffer.getVertexs().size() > m_transformedVertexs.size())
		{
			m_transformedVertexs.resize(m_vertexBuffer.getVertexs().size());
		}

		unsigned int counter = 0;
		for (auto& vertex : m_vertexBuffer.getVertexs())
		{
			auto res = m_modelMatrix * glm::vec4(vertex.m_position[0], vertex.m_position[1], 0.0f, 1.0f);
			m_transformedVertexs[counter].m_position = { res.x, res.y };
			m_transformedVertexs[counter].m_colour = vertex.m_colour;
			m_transformedVertexs[counter].m_texels = vertex.m_texels;

			counter++;
		}

		glNamedBufferSubData(m_vertexBuffer.getID(), 0, m_transformedVertexs.size() * sizeof(qs::Vertex), m_transformedVertexs.data());
	}
}