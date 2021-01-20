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
		PrimitiveVertex::PrimitiveVertex() noexcept
		    : m_pos {0.0f, 0.0f}, m_colour {0.0f, 1.0f, 0.0f, 1.0f}
		{
		}

		PrimitiveVertex::PrimitiveVertex(const float x, const float y, graphics::Colour& col) noexcept
		    : m_pos {x, y}
		{
			m_colour.x = col.r_normal();
			m_colour.y = col.g_normal();
			m_colour.z = col.b_normal();
			m_colour.w = col.a_normal();
		}

		PrimitiveVertex::PrimitiveVertex(const glm::vec2& pos, graphics::Colour& col) noexcept
		    : m_pos {pos}
		{
			m_colour.x = col.r_normal();
			m_colour.y = col.g_normal();
			m_colour.z = col.b_normal();
			m_colour.w = col.a_normal();
		}
	} // namespace graphics
} // namespace galaxy