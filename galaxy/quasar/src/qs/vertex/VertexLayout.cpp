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
	VertexLayout::VertexLayout()
	{
	}

	VertexLayout::VertexLayout(VertexLayout&& vl)
	{
		this->m_attributes = std::move(vl.m_attributes);
		vl.m_attributes.clear();
	}

	VertexLayout& VertexLayout::operator=(VertexLayout&& vl)
	{
		if (this != &vl)
		{
			this->m_attributes = std::move(vl.m_attributes);
			vl.m_attributes.clear();
		}

		return *this;
	}

	VertexLayout::~VertexLayout()
	{
		m_attributes.clear();
	}

	const std::vector<VertexAttribute>& VertexLayout::get_attributes() const
	{
		return m_attributes;
	}
} // namespace qs