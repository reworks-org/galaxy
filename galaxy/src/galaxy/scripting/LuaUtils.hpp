///
/// LuaUtils.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SCRIPTING_LUAUTILS_HPP_
#define GALAXY_SCRIPTING_LUAUTILS_HPP_

#include <functional>

#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace lua
	{
		template<meta::is_class Type>
		struct RegisterWithSol
		{
			inline RegisterWithSol(const std::function<void(void)>& func)
			{
				func();
			}
		};

		template<meta::is_class Type>
		struct Scriptable
		{
			[[nodiscard]] inline void register_with_sol(const std::function<void(void)>& func)
			{
				static RegisterWithSol<Type> rws {func};
			}
		};

		///
		/// Create custom types in lua.
		///
		void register_types();

		///
		/// Allow gui interaction from lua.
		///
		void register_gui();
	} // namespace lua
} // namespace galaxy

#endif