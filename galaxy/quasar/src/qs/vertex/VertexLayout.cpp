///
/// VertexLayout.cpp
/// quasar
///
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

	const std::vector<VertexAttribute>& VertexLayout::get_attributes() const noexcept
	{
		return m_attributes;
	}
} // namespace qs