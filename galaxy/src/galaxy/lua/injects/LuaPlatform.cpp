///
/// LuaPlatform.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <sol/sol.hpp>

#include "galaxy/platform/CPUInfo.hpp"
#include "galaxy/platform/Laptop.hpp"
#include "galaxy/platform/Platform.hpp"
#include "galaxy/platform/Subprocess.hpp"

#include "../Lua.hpp"

namespace galaxy
{
	void Lua::inject_platform() noexcept
	{
		auto& lua = entt::locator<sol::state>::value();

		lua.set("GALAXY_DEBUG_BUILD", GALAXY_DEBUG_BUILD);
		lua.set("GALAXY_EXIT_SUCCESS", GALAXY_EXIT_SUCCESS);

#ifdef GALAXY_WIN_PLATFORM
		lua.set("GALAXY_WIN_PLATFORM", true);
		lua.set("GALAXY_UNIX_PLATFORM", false);
#elif GALAXY_UNIX_PLATFORM
		lua.set("GALAXY_WIN_PLATFORM", false);
		lua.set("GALAXY_UNIX_PLATFORM", true);
#endif

		auto sub_type      = lua.new_usertype<Subprocess>("Subprocess", sol::constructors<Subprocess(), Subprocess(std::string_view)>());
		sub_type["create"] = &Subprocess::create;
		sub_type["wait"]   = &Subprocess::wait;
		sub_type["kill"]   = &Subprocess::kill;

		lua.set_function("galaxy_seed_random", &platform::seed_random);
		lua.set_function("galaxy_set_metadata", &platform::set_metadata);
		lua.set_function("galaxy_set_hint", &platform::set_hint);
		lua.set_function("galaxy_platform_as_string", &platform::as_string);

		// clang-format off
		lua.new_enum<SDL_PowerState>("SDL_PowerState",
		{
			{"SDL_POWERSTATE_ERROR", SDL_PowerState::SDL_POWERSTATE_ERROR},
			{"SDL_POWERSTATE_UNKNOWN", SDL_PowerState::SDL_POWERSTATE_UNKNOWN},
			{"SDL_POWERSTATE_ON_BATTERY", SDL_PowerState::SDL_POWERSTATE_ON_BATTERY},
			{"SDL_POWERSTATE_NO_BATTERY", SDL_PowerState::SDL_POWERSTATE_NO_BATTERY},
			{"SDL_POWERSTATE_CHARGING", SDL_PowerState::SDL_POWERSTATE_CHARGING},
			{"SDL_POWERSTATE_CHARGED", SDL_PowerState::SDL_POWERSTATE_CHARGED}
		});
		// clang-format on

		lua.set_function("galaxy_laptop_get_battery_remaining", &laptop::get_battery_remaining);
		lua.set_function("galaxy_laptop_get_battery_status", &laptop::get_battery_status);

		auto cpu_type                   = lua.new_usertype<CPUInfo>("CPUInfo", sol::constructors<CPUInfo()>());
		cpu_type["cpu_cache_line_size"] = &CPUInfo::cpu_cache_line_size;
		cpu_type["has_altivec"]         = &CPUInfo::has_altivec;
		cpu_type["has_arm_simd"]        = &CPUInfo::has_arm_simd;
		cpu_type["has_avx"]             = &CPUInfo::has_avx;
		cpu_type["has_avx2"]            = &CPUInfo::has_avx2;
		cpu_type["has_avx512f"]         = &CPUInfo::has_avx512f;
		cpu_type["has_lasx"]            = &CPUInfo::has_lasx;
		cpu_type["has_lsx"]             = &CPUInfo::has_lsx;
		cpu_type["has_mmx"]             = &CPUInfo::has_mmx;
		cpu_type["has_neon"]            = &CPUInfo::has_neon;
		cpu_type["has_sse"]             = &CPUInfo::has_sse;
		cpu_type["has_sse2"]            = &CPUInfo::has_sse2;
		cpu_type["has_sse3"]            = &CPUInfo::has_sse3;
		cpu_type["has_sse41"]           = &CPUInfo::has_sse41;
		cpu_type["has_sse42"]           = &CPUInfo::has_sse42;
		cpu_type["has_sve2"]            = &CPUInfo::has_sve2;
		cpu_type["logical_core_count"]  = &CPUInfo::logical_core_count;
		cpu_type["simd_alignment"]      = &CPUInfo::simd_alignment;
		cpu_type["system_page_size"]    = &CPUInfo::system_page_size;
		cpu_type["system_ram"]          = &CPUInfo::system_ram;

		lua.set_function("galaxy_get_cpu_info", &platform::get_cpu_info);
	}
} // namespace galaxy
