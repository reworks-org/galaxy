///
/// LuaPlatform.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <sol/sol.hpp>

#include "galaxy/platform/Pragma.hpp"
#include "galaxy/platform/Subprocess.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_platform()
		{
			auto& lua = entt::locator<sol::state>::value();

#ifdef GALAXY_WIN_PLATFORM
			lua.set("galaxy_win_platform", true);
			lua.set("galaxy_unix_platform", false);
#elif GALAXY_UNIX_PLATFORM
			lua.set("galaxy_win_platform", false);
			lua.set("galaxy_unix_platform", true);
#endif

			auto sub_type         = lua.new_usertype<platform::Subprocess>("Subprocess", sol::constructors<platform::Subprocess(), platform::Subprocess(std::string_view)>());
			sub_type["create"]    = &platform::Subprocess::create;
			sub_type["join"]      = &platform::Subprocess::join;
			sub_type["terminate"] = &platform::Subprocess::terminate;
			sub_type["destroy"]   = &platform::Subprocess::destroy;
			sub_type["alive"]     = &platform::Subprocess::alive;
		}
	} // namespace lua
} // namespace galaxy
