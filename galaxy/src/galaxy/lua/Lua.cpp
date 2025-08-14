///
/// Lua.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Lua.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject()
		{
			inject_logging();
			inject_math();
			inject_platform();
			inject_utils();
		}
	} // namespace lua
} // namespace galaxy
