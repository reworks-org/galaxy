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
		/// Inject everything into Lua.
		///
		void inject();

		///
		/// Injects EnTT into Lua.
		///
		void inject_entt();

		///
		/// Inject Nuklear into Lua.
		///
		void inject_nuklear();

		///
		/// Inject SFML.
		///
		void inject_sfml();

		///
		/// Inject galaxy components into Lua.
		///
		void inject_components();

		///
		/// Inject galaxy core services into Lua.
		///
		void inject_core();

		///
		/// Injects galaxy flag components into Lua.
		///
		void inject_flags();

		///
		/// Injects VFS support into Lua.
		///
		void inject_fs();

		///
		/// Injects input handling into Lua.
		///
		void inject_input();

		///
		/// Injects logging into Lua.
		///
		void inject_logging();

		///
		/// Injects mapping into Lua.
		///
		void inject_map();

		///
		/// Injects math stuff into Lua.
		///
		void inject_math();

		///
		/// Inject custom memory stuff into Lua.
		///
		void inject_mem();

		///
		/// Inject platform stuff into Lua.
		///
		void inject_platform();

		///
		/// Injects galaxy resource caches into lua.
		///
		void inject_resource();

		///
		/// Injects galaxy scene management into lua.
		///
		void inject_scene_control();

		///
		/// Injects galaxy state machine into lua.
		///
		void inject_state();

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
