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

		PrimitiveVertex::PrimitiveVertex(const float x, const float y, const graphics::Colour& col) noexcept
		    : m_pos {x, y}
		{
			auto& col2 = const_cast<graphics::Colour&>(col);
			m_colour.x = col2.r_normal();
			m_colour.y = col2.g_normal();
			m_colour.z = col2.b_normal();
			m_colour.w = col2.a_normal();
		}

		PrimitiveVertex::PrimitiveVertex(const glm::vec2& pos, const graphics::Colour& col) noexcept
		    : m_pos {pos}
		{
			auto& col2 = const_cast<graphics::Colour&>(col);
			m_colour.x = col2.r_normal();
			m_colour.y = col2.g_normal();
			m_colour.z = col2.b_normal();
			m_colour.w = col2.a_normal();
		}
	} // namespace graphics
} // namespace galaxy