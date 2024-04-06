///
/// LuaUtils.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/utils/Globals.hpp"
#include "galaxy/utils/Guid.hpp"
#include "galaxy/utils/StringUtils.hpp"

#include "Lua.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_utils()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();

			lua.set("GALAXY_UPS", GALAXY_UPS);
			lua.set("GALAXY_DT", GALAXY_DT);
			lua.set("GALAXY_FLAG_BITSET_COUNT", GALAXY_FLAG_BITSET_COUNT);
			lua.set("GALAXY_IDENTITY_MATRIX", GALAXY_IDENTITY_MATRIX);
			lua.set("GALAXY_MIN_CAMERA_ZOOM", GALAXY_MIN_CAMERA_ZOOM);
			lua.set("GALAXY_MAX_CAMERA_ZOOM", GALAXY_MAX_CAMERA_ZOOM);
			lua.set("GALAXY_FONT_MSDF_RANGE", GALAXY_FONT_MSDF_RANGE);
			lua.set("GALAXY_FONT_MSDF_SCALE", GALAXY_FONT_MSDF_SCALE);
			lua.set("GALAXY_DEFAULT_ELLIPSE_FRAGMENTS", GALAXY_DEFAULT_ELLIPSE_FRAGMENTS);
			lua.set("GALAXY_WORLD_TO_BOX", GALAXY_WORLD_TO_BOX);
			lua.set("GALAXY_BOX_TO_WORLD", GALAXY_BOX_TO_WORLD);
			lua.set("GALAXY_ROOT_DIR", GALAXY_ROOT_DIR.string());
			lua.set("GALAXY_ASSET_PACK", GALAXY_ASSET_PACK);
			lua.set("GALAXY_APPDATA", GALAXY_APPDATA);
			lua.set("GALAXY_ASSET_DIR", GALAXY_ASSET_DIR);
			lua.set("GALAXY_EDITOR_DATA_DIR", GALAXY_EDITOR_DATA_DIR);
			lua.set("GALAXY_MUSIC_DIR", GALAXY_MUSIC_DIR);
			lua.set("GALAXY_SFX_DIR", GALAXY_SFX_DIR);
			lua.set("GALAXY_VOICE_DIR", GALAXY_VOICE_DIR);
			lua.set("GALAXY_FONT_DIR", GALAXY_FONT_DIR);
			lua.set("GALAXY_SCRIPT_DIR", GALAXY_SCRIPT_DIR);
			lua.set("GALAXY_SHADER_DIR", GALAXY_SHADER_DIR);
			lua.set("GALAXY_ANIM_DIR", GALAXY_ANIM_DIR);
			lua.set("GALAXY_TEXTURE_DIR", GALAXY_TEXTURE_DIR);
			lua.set("GALAXY_LANG_DIR", GALAXY_LANG_DIR);
			lua.set("GALAXY_PREFABS_DIR", GALAXY_PREFABS_DIR);
			lua.set("GALAXY_MAPS_DIR", GALAXY_MAPS_DIR);
			lua.set("GALAXY_VIDEO_DIR", GALAXY_VIDEO_DIR);
			lua.set("GALAXY_UI_DIR", GALAXY_UI_DIR);
			lua.set("GALAXY_UI_FONTS_DIR", GALAXY_UI_FONTS_DIR);
			lua.set("GALAXY_EXIT_SUCCESS", GALAXY_EXIT_SUCCESS);
			lua.set("GALAXY_EXIT_FAILURE", GALAXY_EXIT_FAILURE);

			auto guid_type         = lua.new_usertype<utils::Guid>("Guid", sol::constructors<utils::Guid()>());
			guid_type["as_string"] = &utils::Guid::to_string;
			guid_type["is_empty"]  = &utils::Guid::is_empty;

			/*lua.set_function("galaxy_str_split", &strutils::split);
			lua.set_function("galaxy_str_replace_first", &strutils::replace_first);
			lua.set_function("galaxy_str_replace_all", &strutils::replace_all);
			lua.set_function("galaxy_str_begins_with", &strutils::begins_with);
			lua.set_function("galaxy_str_rtrim", &strutils::rtrim);
			lua.set_function("galaxy_str_ltrim", &strutils::ltrim);
			lua.set_function("galaxy_str_trim", &strutils::trim);
			lua.set_function("galaxy_str_make_single_spaced", &strutils::make_single_spaced);*/
		}
	} // namespace lua
} // namespace galaxy
