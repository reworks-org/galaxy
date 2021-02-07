///
/// LuaUtils.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SCRIPTING_LUAUTILS_HPP_
#define GALAXY_SCRIPTING_LUAUTILS_HPP_

#include <functional>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace lua
	{
		///
		/// Register some of galaxy's loose functions, allowing them to be called from lua.
		///
		void register_functions();

		///
		/// Registers audio classes as lua usertypes.
		///
		void register_audio();

		///
		/// Register ecs with lua.
		///
		void register_ecs();

		///
		/// Register events with lua.
		///
		void register_events();

		///
		/// Registers filesystem with lua.
		///
		void register_fs();

		///
		/// Register graphics related functions.
		///
		void register_graphics();

		///
		/// Register json utils with lua.
		///
		void register_json();

		///
		/// Layer and scene controls.
		///
		void register_layerstack();

		///
		/// Register math functions.
		///
		void register_math();

		///
		/// Register physics.
		///
		void register_physics();

		///
		/// Register resource functions.
		///
		void register_res();
	} // namespace lua
} // namespace galaxy

#endif