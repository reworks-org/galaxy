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
	
	void Sprite::create(const qs::BufferType bufferType, const unsigned int quadCount) noexcept
	{
		qs::Vertex tl;
		tl.m_position = { 0.0f, 0.0f };
		tl.m_colour = { 0.0f, 0.0f, 0.0f, 1.0f };
		tl.m_texels = { 0.0f, 1.0f };

		qs::Vertex tr;
		tr.m_position = { static_cast<float>(m_width), 0.0f };
		tr.m_colour = { 0.0f, 0.0f, 0.0f, 1.0f };
		tr.m_texels = { 1.0f, 1.0f };

		qs::Vertex br;
		br.m_position = { static_cast<float>(m_width), static_cast<float>(m_height) };
		br.m_colour = { 0.0f, 0.0f, 0.0f, 1.0f };
		br.m_texels = { 1.0f, 0.0f };

		qs::Vertex bl;
		bl.m_position = { 0.0f, static_cast<float>(m_height) };
		bl.m_colour = { 0.0f, 0.0f, 0.0f, 1.0f };
		bl.m_texels = { 0.0f, 0.0f };

		m_vertexBuffer.create({ tl, tr, br, bl}, bufferType, quadCount);

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

		glNamedBufferSubData(m_id, 0, m_transformedVertexs.size() * sizeof(qs::Vertex), m_transformedVertexs.data());
	}
}