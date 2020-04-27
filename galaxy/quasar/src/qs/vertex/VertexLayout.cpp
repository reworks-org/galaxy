///
/// VertexLayout.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

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

	const std::vector<qs::VertexAttribute>& VertexLayout::getAttributes() const noexcept
	{
		return m_attributes;
	}
}