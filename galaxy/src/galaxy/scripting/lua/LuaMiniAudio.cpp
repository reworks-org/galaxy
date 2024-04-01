///
/// LuaMiniAudio.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <miniaudio.h>
#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"

#include "Lua.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_miniaudio()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();

			// clang-format off
			lua.new_enum<ma_pan_mode>("ma_pan_mode",
			{
				{"ma_pan_mode_balance", ma_pan_mode_balance},
				{"ma_pan_mode_pan", ma_pan_mode_pan}
			});

			lua.new_enum<ma_positioning>("ma_positioning",
            {
				{"ma_positioning_absolute", ma_positioning_absolute},
				{"ma_positioning_relative", ma_positioning_relative}
            });

			lua.new_enum<ma_attenuation_model>("ma_attenuation_model",
            {
				{"ma_attenuation_model_none", ma_attenuation_model_none},
				{"ma_attenuation_model_inverse", ma_attenuation_model_inverse},
				{"ma_attenuation_model_linear", ma_attenuation_model_linear},
				{"ma_attenuation_model_exponential", ma_attenuation_model_exponential}
            });
			// clang-format on

			auto ma_vec3f_type = lua.new_usertype<ma_vec3f>(sol::constructors<ma_vec3f()>());
			ma_vec3f_type["x"] = &ma_vec3f::x;
			ma_vec3f_type["y"] = &ma_vec3f::y;
			ma_vec3f_type["z"] = &ma_vec3f::z;
		}
	} // namespace lua
} // namespace galaxy
