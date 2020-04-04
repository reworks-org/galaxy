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
		:m_stride(0), m_totalOffset(0)
	{
	}

	VertexLayout::VertexLayout(const unsigned int stride) noexcept
		:m_stride(stride), m_totalOffset(0)
	{
	}

	VertexLayout::~VertexLayout() noexcept
	{
		m_attributes.clear();
	}

	void VertexLayout::setStride(const unsigned int stride) noexcept
	{
		m_stride = stride;
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