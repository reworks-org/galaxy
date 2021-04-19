///
/// Point.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Point.hpp"

namespace galaxy
{
	namespace light
	{
		Point::Point() noexcept
		    : Light {}, m_pos {0.0f, 0.0f, 0.0f}
		{
		}
	} // namespace light
} // namespace galaxy