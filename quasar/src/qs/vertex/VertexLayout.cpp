///
/// VertexLayout.cpp
/// Quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include "VertexLayout.hpp"

namespace qs
{
	VertexLayout::VertexLayout(unsigned int stride) noexcept
		:m_stride(stride), m_totalOffset(0)
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

	const unsigned int VertexLayout::stride() const
	{
		return m_stride;
	}
}