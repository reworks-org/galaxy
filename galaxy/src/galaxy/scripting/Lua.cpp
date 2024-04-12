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
			inject_miniaudio();
			inject_glm();
			inject_entt();
			inject_box2d();
			inject_nuklear();
			inject_components();
			inject_core();
			inject_error();
			inject_events();
			inject_flags();
			inject_fs();
			inject_graphics();
			inject_input();
			inject_math();
			inject_media();
			inject_resources();
			inject_scenes();
			inject_states();
			inject_utils();
			inject_services();
		}
	} // namespace lua
} // namespace galaxy
