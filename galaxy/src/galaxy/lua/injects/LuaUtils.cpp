///
/// LuaUtils.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <sol/sol.hpp>

#include "galaxy/utils/StringUtils.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_utils()
		{
			auto& lua = entt::locator<sol::state>::value();

			lua.set_function("str_split", &str::split);
			lua.set_function("str_replace_first", &str::replace_first);
			lua.set_function("str_replace_all", &str::replace_all);
			lua.set_function("str_begins_with", &str::begins_with);
			lua.set_function("str_rtrim", &str::rtrim);
			lua.set_function("str_ltrim", &str::ltrim);
			lua.set_function("str_trim", &str::trim);
			lua.set_function("str_make_single_spaced", &str::make_single_spaced);
		}
	} // namespace lua
} // namespace galaxy
