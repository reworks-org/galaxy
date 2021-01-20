///
/// WindowResized.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "WindowResized.hpp"

namespace galaxy
{
	namespace events
	{
		WindowResized::WindowResized() noexcept
		    : m_width {0}, m_height {0}
		{
		}

		WindowResized::WindowResized(const int new_width, const int new_height) noexcept
		    : m_width {new_width}, m_height {new_height}
		{
		}
	} // namespace events
} // namespace galaxy