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
		/// Inject miniaudio into lua.
		///
		void inject_miniaudio();

		///
		/// Injects GLM into lua.
		///
		void inject_glm();

		///
		/// Injects EnTT into lua.
		///
		void inject_entt();

		///
		/// Injects Box2D into lua.
		///
		void inject_box2d();

		///
		/// Inject Nuklear into Lua.
		///
		void inject_nuklear();

		///
		/// Inject galaxy components into lua.
		///
		void inject_components();

		///
		/// Inject galaxy core services into lua.
		///
		void inject_core();

		///
		/// Injects error handling into lua.
		///
		void inject_error();

		///
		/// Injects galaxy events into lua.
		///
		void inject_events();

		///
		/// Injects galaxy flag components into lua.
		///
		void inject_flags();

		///
		/// Injects VFS support into lua.
		///
		void inject_fs();

		///
		/// Injects graphics primitives, not routines, into lua.
		///
		void inject_graphics();

		///
		/// Injects input handling into lua.
		///
		void inject_input();

		///
		/// Injects math stuff into lua.
		///
		void inject_math();

		///
		/// Injects video and audio support into lua.
		///
		void inject_media();

		///
		/// Injects galaxy resource caches into lua.
		///
		void inject_resources();

		///
		/// Injects galaxy scene management into lua.
		///
		void inject_scenes();

		///
		/// Injects galaxy state machine into lua.
		///
		void inject_states();

		///
		/// Injects misc galaxy utils into lua.
		///
		void inject_utils();

		///
		/// Regsiter static engine services in lua.
		///
		void inject_services();

	} // namespace lua
} // namespace galaxy

#endif
