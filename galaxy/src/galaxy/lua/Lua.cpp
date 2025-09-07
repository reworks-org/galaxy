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
			inject_core();
			inject_fs();
			inject_input();
			inject_logging();
			inject_math();
			inject_platform();
			inject_time();
			inject_utils();
		}
	} // namespace lua
} // namespace galaxy
