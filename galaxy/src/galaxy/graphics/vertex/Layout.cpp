///
/// Layout.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Layout.hpp"

namespace galaxy
{
	namespace graphics
	{
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