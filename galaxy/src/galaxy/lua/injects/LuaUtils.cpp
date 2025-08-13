///
/// LuaUtils.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <sol/sol.hpp>

#include "galaxy/utils/Guid.hpp"
#include "galaxy/utils/StringUtils.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_utils()
		{
			auto& lua = entt::locator<sol::state>::value();

			auto guid_type         = lua.new_usertype<utils::Guid>("Guid", sol::constructors<utils::Guid()>());
			guid_type["as_string"] = &utils::Guid::to_string;

			lua.set_function("str_split", &strutils::split);
			// lua.set_function("str_replace_first", &strutils::replace_first);
			// lua.set_function("str_replace_all", &strutils::replace_all);
			lua.set_function("str_begins_with", &strutils::begins_with);
			// lua.set_function("str_rtrim", &strutils::rtrim);
			// lua.set_function("str_ltrim", &strutils::ltrim);
			// lua.set_function("str_trim", &strutils::trim);
			// lua.set_function("str_make_single_spaced", &strutils::make_single_spaced);
		}
	} // namespace lua
} // namespace galaxy
