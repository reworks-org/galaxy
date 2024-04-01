///
/// Lua.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SCRIPTING_LUA_HPP_
#define GALAXY_SCRIPTING_LUA_HPP_

namespace galaxy
{
	namespace lua
	{
		///
		/// Inject everything into lua.
		///
		void inject();

		///
		///
		///
		void inject_miniaudio();

		///
		///
		///
		void inject_glm();

		///
		///
		///
		void inject_entt();

		///
		///
		///
		void inject_box2d();

		///
		/// Load nuklear ui into lua.
		///
		void inject_nuklear();

		///
		///
		///
		void inject_components();

		///
		///
		///
		void inject_core();

		///
		///
		///
		void inject_error();

		///
		///
		///
		void inject_events();

		///
		///
		///
		void inject_flags();

		///
		///
		///
		void inject_fs();

		///
		///
		///
		void inject_graphics();

		///
		///
		///
		void inject_input();

		///
		///
		///
		void inject_math();

		///
		///
		///
		void inject_media();

		///
		///
		///
		void inject_resources();

		///
		///
		///
		void inject_scenes();

		///
		///
		///
		void inject_utils();

		///
		///
		///

		///
		/// Regsiter static engine services into lua.
		///
		void inject_services();

	} // namespace lua
} // namespace galaxy

#endif
