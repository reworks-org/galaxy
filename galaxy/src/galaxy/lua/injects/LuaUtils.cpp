///
/// LuaUtils.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <sol/sol.hpp>

#include "galaxy/utils/StringUtils.hpp"
#include "galaxy/utils/UUID.hpp"

#include "../Lua.hpp"

namespace galaxy
{
	void Lua::inject_utils() noexcept
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

		auto uuid_type    = lua.new_usertype<UUID>("UUID", sol::constructors<UUID()>());
		uuid_type["hash"] = &UUID::hash;
		uuid_type["str"]  = &UUID::str;
	}
} // namespace galaxy
