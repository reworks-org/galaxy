///
/// PrimitiveVertex.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "PrimitiveVertex.hpp"

namespace galaxy
{
	namespace graphics
	{
		PrimitiveVertex::PrimitiveVertex()
		    : m_pos {0.0f, 0.0f}, m_colour {0, 0, 0, 255}
		{
		}

		PrimitiveVertex::PrimitiveVertex(const float x, const float y, graphics::Colour& col)
		    : m_pos {x, y}
		{
			auto arr   = col.get_normalized();
			m_colour.x = arr[0];
			m_colour.y = arr[1];
			m_colour.z = arr[2];
			m_colour.w = arr[3];
		}
	} // namespace graphics
} // namespace galaxy