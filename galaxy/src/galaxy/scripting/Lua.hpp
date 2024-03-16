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
		/// Load some provided external libs into lua.
		///
		void inject_external_modules();

		///
		/// Load the dependencies galaxy uses into lua.
		///
		void inject_dependencies();

		///
		/// Load nuklear ui into lua.
		///
		void inject_nuklear();

		///
		/// Registers galaxy_engine into Lua.
		///
		void inject_galaxy();

		///
		/// Regsiter static engine services into lua.
		///
		void inject_services();

	} // namespace lua
} // namespace galaxy

#endif
