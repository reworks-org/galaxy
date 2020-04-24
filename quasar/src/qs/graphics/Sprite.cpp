///
/// Sprite.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <cstdarg>

#include "qs/utils/Utility.hpp"

#include "Sprite.hpp"

///
/// Core namespace.
///
namespace qs
{
	Sprite::Sprite() noexcept
		:Texture(), m_activeOffset(0), m_activeTransform(nullptr)
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

		m_transforms.emplace(0, std::move(qs::Transform()));
		m_transforms[0].setRotationOrigin(static_cast<float>(m_width), static_cast<float>(m_height));

		setActiveQuad();
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

		m_transforms.emplace(0, std::move(qs::Transform()));
		m_transforms[0].setRotationOrigin(texSrc.m_width, texSrc.m_height);

		setActiveQuad();
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

			m_transforms.emplace(increment, qs::Transform());

			auto pos = m_vertexBuffer.getVertexs()[2].m_position;
			m_transforms[increment].setRotationOrigin(pos[0] * 0.5f, pos[1] * 0.5f);

			increment += 4;
		}
		m_indexBuffer.create<qs::DynamicBufferType>(indexs);

		m_layout.add<qs::PositionVAType>(2);
		m_layout.add<qs::ColourVAType>(4);
		m_layout.add<qs::TexelVAType>(2);

		m_vertexArray.create(m_vertexBuffer, m_indexBuffer, m_layout);

		setActiveQuad();
	}

	void Sprite::setActiveQuad(const int offset)
	{
		m_activeOffset = offset;
		m_activeTransform = &m_transforms[m_activeOffset];
	}

	void Sprite::move(const float x, const float y)
	{
		m_activeTransform->move(x, y);
	}

	void Sprite::rotate(const float degrees) noexcept
	{
		m_activeTransform->rotate(degrees);
	}

	void Sprite::scale(const float scale) noexcept
	{
		m_activeTransform->scale(scale);
	}

	void Sprite::setOpacity(float opacity) noexcept
	{
		if (opacity > 1.0f)
		{
			opacity = 1.0f;
		}
		else if (opacity < 0.0f)
		{
			opacity = 0.0f;
		}

		auto& vs = m_vertexBuffer.getVertexs();
		vs[m_activeOffset + 0].m_colour[3] = opacity;
		vs[m_activeOffset + 1].m_colour[3] = opacity;
		vs[m_activeOffset + 2].m_colour[3] = opacity;
		vs[m_activeOffset + 3].m_colour[3] = opacity;
	}

	void Sprite::applyTransforms()
	{
		// Have to update transforms first.
		m_transformVertexs = m_vertexBuffer.getVertexs();

		for (auto& tf : m_transforms)
		{
			qs::Vertex* vertex = &m_transformVertexs[tf.first + 0];
			auto res = tf.second.getTransformation() * glm::vec4(vertex->m_position[0], vertex->m_position[1], 0.0f, 1.0f);
			vertex->m_position = { res.x, res.y };

			vertex = &m_transformVertexs[tf.first + 1];
			res = tf.second.getTransformation() * glm::vec4(vertex->m_position[0], vertex->m_position[1], 0.0f, 1.0f);
			vertex->m_position = { res.x, res.y };

			vertex = &m_transformVertexs[tf.first + 2];
			res = tf.second.getTransformation() * glm::vec4(vertex->m_position[0], vertex->m_position[1], 0.0f, 1.0f);
			vertex->m_position = { res.x, res.y };

			vertex = &m_transformVertexs[tf.first + 3];
			res = tf.second.getTransformation() * glm::vec4(vertex->m_position[0], vertex->m_position[1], 0.0f, 1.0f);
			vertex->m_position = { res.x, res.y };
		}

		glNamedBufferSubData(m_vertexBuffer.getID(), 0, m_transformVertexs.size() * sizeof(qs::Vertex), m_transformVertexs.data());
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

	qs::VertexArray& Sprite::getVAO() noexcept
	{
		return m_vertexArray;
	}

	const unsigned int Sprite::getCount() const noexcept
	{
		return m_indexBuffer.getCount();
	}
}