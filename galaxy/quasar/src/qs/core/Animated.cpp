///
/// Animated.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "Animated.hpp"

///
/// Core namespace.
///
namespace qs
{
	Animated::Animated() noexcept
	    : m_dirty_texels {false}, m_texels {0.0f, 0.0f}
	{
	}

	void Animated::set_texels(const float x, const float y) noexcept
	{
		m_texels.x     = x;
		m_texels.y     = y;
		m_dirty_texels = true;
	}

	void Animated::move_texels(const float x, const float y) noexcept
	{
		m_texels.x += x;
		m_texels.y += y;
		m_dirty_texels = true;
	}

	glm::vec2& Animated::get_texels() noexcept
	{
		return m_texels;
	}
} // namespace qs