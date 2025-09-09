///
/// LuaTime.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <sol/sol.hpp>

#include "galaxy/time/Stopwatch.hpp"
#include "galaxy/time/Time.hpp"
#include "galaxy/time/Timer.hpp"

#include "../Lua.hpp"

namespace galaxy
{
	void Lua::inject_time() noexcept
	{
		auto& lua = entt::locator<sol::state>::value();

		lua.set_function("time_now", &time::now);
		lua.set_function("time_set_dt", sol::resolve<void(const double)>(&time::dt));
		lua.set_function("time_get_dt", sol::resolve<double(void)>(&time::dt));

		auto stopwatch_type = lua.new_usertype<Stopwatch>("Stopwatch", sol::constructors<Stopwatch()>());
		// stopwatch_type["set"]             = &Stopwatch::set;
		stopwatch_type["repeat"]          = &Stopwatch::repeat;
		stopwatch_type["start"]           = &Stopwatch::start;
		stopwatch_type["stop"]            = &Stopwatch::stop;
		stopwatch_type["pause"]           = &Stopwatch::pause;
		stopwatch_type["unpause"]         = &Stopwatch::unpause;
		stopwatch_type["update"]          = &Stopwatch::update;
		stopwatch_type["stopped"]         = &Stopwatch::stopped;
		stopwatch_type["paused"]          = &Stopwatch::paused;
		stopwatch_type["get_time_passed"] = &Stopwatch::get_time_passed;

		auto timer_type = lua.new_usertype<Timer>("Timer", sol::constructors<Timer()>());
		// timer_type["set"]     = &Timer::set;
		timer_type["start"]   = &Timer::start;
		timer_type["stop"]    = &Timer::stop;
		timer_type["pause"]   = &Timer::pause;
		timer_type["stopped"] = &Timer::stopped;
		timer_type["paused"]  = &Timer::paused;
	}
} // namespace galaxy
