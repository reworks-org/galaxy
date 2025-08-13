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
			lua.set("GALAXY_WIN_PLATFORM", true);
			lua.set("GALAXY_UNIX_PLATFORM", false);
#elif GALAXY_UNIX_PLATFORM
			lua.set("GALAXY_WIN_PLATFORM", false);
			lua.set("GALAXY_UNIX_PLATFORM", true);
#endif

			auto sub_type         = lua.new_usertype<Subprocess>("Subprocess", sol::constructors<Subprocess(), Subprocess(std::string_view)>());
			sub_type["create"]    = &Subprocess::create;
			sub_type["join"]      = &Subprocess::join;
			sub_type["terminate"] = &Subprocess::terminate;
			sub_type["destroy"]   = &Subprocess::destroy;
			sub_type["alive"]     = &Subprocess::alive;
		}
	} // namespace lua
} // namespace galaxy
