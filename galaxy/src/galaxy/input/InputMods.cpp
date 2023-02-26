///
/// InputMods.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "InputMods.hpp"

namespace galaxy
{
	namespace input
	{
		input::InputMods int_to_keymod(const int mod) noexcept
		{
			return static_cast<input::InputMods>(mod);
		}
	} // namespace input
} // namespace galaxy