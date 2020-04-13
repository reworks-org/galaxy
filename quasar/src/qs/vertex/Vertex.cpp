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
	std::array<qs::Vertex, 4>&& Vertex::make_quad(const protostar::Rect<float>& bounds, const std::array<float, 4>& col, const float u, const float v)
	{
		return std::move(std::array<qs::Vertex, 4>(
			{ 
				qs::Vertex{bounds.m_x, bounds.m_y, col, u, v},
				qs::Vertex{bounds.m_x + bounds.m_width, bounds.m_y, col, u + bounds.m_width, v},
				qs::Vertex{bounds.m_x + bounds.m_width, bounds.m_y + bounds.m_height, col, u + bounds.m_width, v + bounds.m_height},
				qs::Vertex{bounds.m_x, bounds.m_y + bounds.m_height, col, u, v + bounds.m_height}
			}));
	}

	Vertex::Vertex() noexcept
		:m_position{0.0f, 0.0f}, m_colour{0.0f, 0.0f, 0.0f, 1.0f}, m_texels{0.0f, 0.0f}
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

	Vertex::Vertex(const float x, const float y, const std::array<float, 4>& col, const float u, const float v)
		:m_position{ x, y }, m_colour(col), m_texels{ u, v }
	{
	}
}