///
/// InstanceVertex.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "InstanceVertex.hpp"

///
/// Core namespace.
///
namespace qs
{
	InstanceVertex::InstanceVertex() noexcept
		:m_pos{0.0f, 0.0f}, m_texels{0.0f, 0.0f}, m_offset{0.0f, 0.0f}
	{
	}

	InstanceVertex::InstanceVertex(const float x, const float y, const float u, const float v, const float off_x, const float off_y) noexcept
		:m_pos{ x, y }, m_texels{ u, v }, m_offset{ off_x, off_y }
	{
	}

}