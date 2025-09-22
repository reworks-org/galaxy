///
/// Lua.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Lua.hpp"

namespace galaxy
{
	void Lua::inject() noexcept
	{
		Lua::inject_core();
		Lua::inject_entt();
		Lua::inject_events();
		Lua::inject_fs();
		Lua::inject_glm();
		Lua::inject_input();
		Lua::inject_logging();
		Lua::inject_math();
		Lua::inject_platform();
		Lua::inject_scene();
		Lua::inject_state();
		Lua::inject_time();
		Lua::inject_utils();

		Lua::inject_services();
	}
} // namespace galaxy
