///
/// Keys.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Keys.hpp"

namespace galaxy
{
	namespace input
	{
		input::Keys int_to_key(const int key) noexcept
		{
			return static_cast<input::Keys>(key);
		}
	} // namespace input
} // namespace galaxy