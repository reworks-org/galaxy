///
/// BatchSprite2D.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <cstdarg>
#include <array>

#include "BatchSprite2D.hpp"

///
/// Core namespace.
///
namespace qs
{
	BatchSprite2D::BatchSprite2D() noexcept
		:Texture(), m_counter(0)
	{
	}

	BatchSprite2D::~BatchSprite2D() noexcept
	{
		m_vertexArray.unbind();
		unbind(); // texture
		glDeleteTextures(1, &m_id);
		m_rawTransforms.clear();
		m_transforms.clear();
		m_nameToID.clear();
	}

	void BatchSprite2D::push(const std::array<float, 20>& vertexs) noexcept
	{
		for (const float vertex : vertexs)
		{
			m_vBuffer.emplace_back(vertex);

			m_iBuffer.push_back(0 + m_counter);
			m_iBuffer.push_back(1 + m_counter);
			m_iBuffer.push_back(3 + m_counter);
			m_iBuffer.push_back(1 + m_counter);
			m_iBuffer.push_back(2 + m_counter);
			m_iBuffer.push_back(3 + m_counter);

			m_counter += 4;
		}
	}

	void BatchSprite2D::create(const int totalSprites) noexcept
	{
		m_vertexBuffer.create(m_vBuffer);
		m_indexBuffer.create(m_iBuffer);

		m_layout.setStride(5);
		m_layout.add<float>(2);
		m_layout.add<float>(2);
		m_layout.add<float>(1);

		m_vertexArray.create(m_vertexBuffer, m_layout);
		m_vertexArray.addIndexBuffer(m_indexBuffer);

		m_transforms = std::vector<qs::Transform>(totalSprites);
	}

	void BatchSprite2D::activate() noexcept
	{
		bind(); // texture
		m_vertexArray.bind();
	}

	const unsigned int BatchSprite2D::count() const noexcept
	{
		return m_indexBuffer.count();
	}

	void BatchSprite2D::addID(const std::string& name, const int id) noexcept
	{
		m_nameToID.emplace(name, id);
	}

	const int BatchSprite2D::getID(const std::string& texture) noexcept
	{
		return m_nameToID[texture];
	}

	qs::Transform& BatchSprite2D::getTransform(const std::string& name)
	{
		return m_transforms[m_nameToID[name]];
	}

	std::vector<glm::mat4>& BatchSprite2D::getTransformArray()
	{
		m_rawTransforms.clear(); //preserves memory, deletes contents.

		for (auto& transform : m_transforms)
		{
			m_rawTransforms.push_back(transform.getTransformation());
		}

		return m_rawTransforms;
	}
}