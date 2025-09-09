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
	///
	/// Just a wrapper around static injection functions.
	///
	class Lua
	{
	public:
		///
		/// Default destructor.
		///
		~Lua() = default;

	private:
		///
		/// Not supposed to be created.
		///
		Lua() = delete;

	public:
		///
		/// Inject everything into Lua.
		///
		static void inject() noexcept;

	private:
		///
		/// Inject galaxy core services into Lua.
		///
		static void inject_core() noexcept;

		///
		/// Inject galaxy event data objects into Lua.
		///
		static void inject_events() noexcept;

		///
		/// Injects VFS support into Lua.
		///
		static void inject_fs() noexcept;

		///
		/// Injects input handling into Lua.
		///
		static void inject_input() noexcept;

		///
		/// Injects logging into Lua.
		///
		static void inject_logging() noexcept;

		///
		/// Injects math stuff into Lua.
		///
		static void inject_math() noexcept;

		///
		/// Inject platform stuff into Lua.
		///
		static void inject_platform() noexcept;

		///
		/// Regsiter static engine services in Lua.
		///
		static void inject_services() noexcept;

		///
		/// Register galaxy time support functions into Lua.
		///
		static void inject_time() noexcept;

		///
		/// Injects misc galaxy utils into Lua.
		///
		static void inject_utils() noexcept;
	};
} // namespace galaxy

/*
///
/// Injects EnTT into Lua.
///
void inject_entt();

///
/// Inject Nuklear into Lua.
///
void inject_nuklear();

///
/// Inject galaxy components into Lua.
///
void inject_components();

///
/// Injects galaxy flag components into Lua.
///
void inject_flags();

///
/// Injects mapping into Lua.
///
void inject_map();

///
/// Inject custom memory stuff into Lua.
///
void inject_mem();

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
*/

#endif
