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
			inject_entt();
			// inject_nuklear();
			// inject_sfml();
			// inject_components();
			// inject_core();
			inject_flags();
			inject_fs();
			// inject_input();
			inject_logging();
			// inject_map();
			inject_math();
			inject_mem();
			inject_platform();
			// inject_resource();
			inject_scene_control();
			// inject_state();
			inject_utils();
			inject_services();
		}
	} // namespace lua
} // namespace galaxy
