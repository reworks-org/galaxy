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
		VertexLayout::VertexLayout()
		{
		}

		VertexLayout::~VertexLayout()
		{
			m_attributes.clear();
		}

		const std::vector<VertexAttribute>& VertexLayout::get_attributes() const
		{
			return m_attributes;
		}
	} // namespace graphics
} // namespace galaxy