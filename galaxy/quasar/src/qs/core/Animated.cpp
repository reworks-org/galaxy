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
		:m_areTexelsDirty(false), m_texels(0.0f, 0.0f)
	{
	}

	void Animated::setTexels(const float x, const float y) noexcept
	{
		m_texels.x = x;
		m_texels.y = y;
		m_areTexelsDirty = true;
	}

	void Animated::moveTexels(const float x, const float y) noexcept
	{
		m_texels.x += x;
		m_texels.y += y;
		m_areTexelsDirty = true;
	}

	glm::vec2& Animated::getTexels() noexcept
	{
		return m_texels;
	}
}