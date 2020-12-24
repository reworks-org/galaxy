///
/// MouseMoved.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "MouseMoved.hpp"

namespace galaxy
{
	namespace events
	{
		MouseMoved::MouseMoved()
		    : m_x {0.0}, m_y {0.0}
		{
		}

		MouseMoved::MouseMoved(const double x, const double y)
		    : m_x {x}, m_y {y}
		{
		}
	} // namespace events
} // namespace galaxy