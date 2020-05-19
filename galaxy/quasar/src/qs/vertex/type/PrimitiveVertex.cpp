///
/// PrimitiveVertex.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "PrimitiveVertex.hpp"

///
/// Core namespace.
///
namespace qs
{
	PrimitiveVertex::PrimitiveVertex() noexcept
		:m_pos(0.0f, 0.0f), m_colour(0.0f, 0.0f, 0.0f, 1.0f)
	{
	}

	PrimitiveVertex::PrimitiveVertex(const float x, const float y, protostar::Colour& col) noexcept
		:m_pos(x, y), m_colour(col)
	{
	}
}