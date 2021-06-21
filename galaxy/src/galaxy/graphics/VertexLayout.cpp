///
/// VertexLayout.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "VertexLayout.hpp"

namespace galaxy
{
	namespace graphics
	{
		VertexLayout::VertexLayout(VertexLayout&& vl) noexcept
		{
			this->m_attributes = std::move(vl.m_attributes);
		}

		VertexLayout& VertexLayout::operator=(VertexLayout&& vl) noexcept
		{
			if (this != &vl)
			{
				this->m_attributes = std::move(vl.m_attributes);
			}

			return *this;
		}

		VertexLayout::~VertexLayout() noexcept
		{
			m_attributes.clear();
		}

		const std::vector<VertexAttribute>& VertexLayout::get_attributes() const noexcept
		{
			return m_attributes;
		}
	} // namespace graphics
} // namespace galaxy