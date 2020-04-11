///
/// Vertex.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include "qs/utils/Utility.hpp"

#include "Vertex.hpp"

///
/// Core namespace.
///
namespace qs
{
	Vertex::Vertex() noexcept
		:m_position{0.0f, 0.0f}, m_colour{0.0f, 0.0f, 0.0f, 0.0f}, m_texels{0.0f, 0.0f}
	{
	}
	
	Vertex::Vertex(const float x, const float y, protostar::Colour& col, const float u, const float v)
		:m_position{x, y}, m_texels{u, v}
	{
		m_colour[0] = qs::Utils::uint8ToFloat(col.m_red);
		m_colour[1] = qs::Utils::uint8ToFloat(col.m_green);
		m_colour[2] = qs::Utils::uint8ToFloat(col.m_blue);
		m_colour[3] = qs::Utils::uint8ToFloat(col.m_alpha);
	}
}