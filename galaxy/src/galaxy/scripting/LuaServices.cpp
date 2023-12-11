///
/// LuaServices.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include "galaxy/core/Config.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/media/AudioEngine.hpp"
#include "galaxy/meta/EntityMeta.hpp"
#include "galaxy/resource/BasicScripts.hpp"
#include "galaxy/resource/Fonts.hpp"
#include "galaxy/resource/Language.hpp"
#include "galaxy/resource/Maps.hpp"
#include "galaxy/resource/Media.hpp"
#include "galaxy/resource/Prefabs.hpp"
#include "galaxy/resource/Shaders.hpp"
#include "galaxy/resource/TextureAtlas.hpp"
#include "galaxy/scene/SceneManager.hpp"
#include "galaxy/ui/NuklearUI.hpp"

#include "Lua.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_services()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();

			lua["galaxy_basicscripts"]  = std::ref(core::ServiceLocator<resource::BasicScripts>::ref());
			lua["galaxy_fonts"]         = std::ref(core::ServiceLocator<resource::Fonts>::ref());
			lua["galaxy_soundengine"]   = std::ref(core::ServiceLocator<media::SoundEngine>::ref());
			lua["galaxy_voiceengine"]   = std::ref(core::ServiceLocator<media::VoiceEngine>::ref());
			lua["galaxy_musicengine"]   = std::ref(core::ServiceLocator<media::MusicEngine>::ref());
			lua["galaxy_sfx"]           = std::ref(core::ServiceLocator<resource::SFXCache>::ref());
			lua["galaxy_voice"]         = std::ref(core::ServiceLocator<resource::VoiceCache>::ref());
			lua["galaxy_music"]         = std::ref(core::ServiceLocator<resource::MusicCache>::ref());
			lua["galaxy_videos"]        = std::ref(core::ServiceLocator<resource::VideoCache>::ref());
			lua["galaxy_prefabs"]       = std::ref(core::ServiceLocator<resource::Prefabs>::ref());
			lua["galaxy_shaders"]       = std::ref(core::ServiceLocator<resource::Shaders>::ref());
			lua["galaxy_textureatlas"]  = std::ref(core::ServiceLocator<resource::TextureAtlas>::ref());
			lua["galaxy_nui"]           = std::ref(core::ServiceLocator<ui::NuklearUI>::ref());
			lua["galaxy_state_manager"] = std::ref(core::ServiceLocator<scene::SceneManager>::ref());
			lua["galaxy_entitymeta"]    = std::ref(core::ServiceLocator<meta::EntityMeta>::ref());
			lua["galaxy_fs"]            = std::ref(core::ServiceLocator<fs::VirtualFileSystem>::ref());
			lua["galaxy_config"]        = std::ref(core::ServiceLocator<core::Config>::ref());
			lua["galaxy_language"]      = std::ref(core::ServiceLocator<resource::Language>::ref());
			lua["galaxy_maps"]          = std::ref(core::ServiceLocator<resource::Maps>::ref());
		}
	} // namespace lua
} // namespace galaxy
