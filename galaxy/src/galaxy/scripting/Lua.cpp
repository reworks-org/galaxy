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
			lua::inject_miniaudio();
			lua::inject_glm();
			lua::inject_entt();
			lua::inject_box2d();
			lua::inject_nuklear();
			lua::inject_components();
			lua::inject_core();
			lua::inject_error();
			lua::inject_events();
			lua::inject_flags();
			lua::inject_fs();
			lua::inject_graphics();
			lua::inject_input();
			lua::inject_math();
			lua::inject_media();
			lua::inject_resources();
			lua::inject_scenes();
			lua::inject_utils();
			lua::inject_services();
		}
	} // namespace lua
} // namespace galaxy
