///
/// VertexLayout.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "qs/vertex/Vertex.hpp"

#include "VertexLayout.hpp"

///
/// Core namespace.
///
namespace qs
{
	VertexLayout::VertexLayout() noexcept
	{
	}

	VertexLayout::~VertexLayout() noexcept
	{
		m_attributes.clear();
	}

	void VertexLayout::add(const int size, const qs::VertexAttribute::Type type)
	{
		if (type == qs::VertexAttribute::Type::POSITION)
		{
			m_attributes.emplace_back(size, GL_FLOAT, GL_FALSE, offsetof(qs::Vertex, m_position));
		}
		else if (type == qs::VertexAttribute::Type::COLOUR)
		{
			m_attributes.emplace_back(size, GL_FLOAT, GL_FALSE, offsetof(qs::Vertex, m_colour));
		}
		else if (type == qs::VertexAttribute::Type::TEXELS)
		{
			m_attributes.emplace_back(size, GL_FLOAT, GL_FALSE, offsetof(qs::Vertex, m_texels));
		}
	}

	const std::vector<qs::VertexAttribute>& VertexLayout::getAttributes() const noexcept
	{
		return m_attributes;
	}
}