///
/// MouseMoved.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "MouseMoved.hpp"

namespace galaxy
{
	MouseMoved::MouseMoved(const float xpos, const float ypos, const float xrel, const float yrel) noexcept
		: m_xpos {xpos}
		, m_ypos {ypos}
		, m_xrel {xrel}
		, m_yrel {yrel}
	{
	}
} // namespace galaxy
