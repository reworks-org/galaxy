///
/// LuaServices.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/signal/dispatcher.hpp>
#include <sol/sol.hpp>

#include "galaxy/core/Config.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/media/AudioEngine.hpp"
#include "galaxy/meta/EntityMeta.hpp"
#include "galaxy/resource/Fonts.hpp"
#include "galaxy/resource/Media.hpp"
#include "galaxy/resource/Prefabs.hpp"
#include "galaxy/resource/Scripts.hpp"
#include "galaxy/resource/Shaders.hpp"
#include "galaxy/resource/Textures.hpp"
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

			// thead_pool
			lua["galaxy_config"] = std::ref(core::ServiceLocator<core::Config>::ref());
			// window
			lua["galaxy_fs"]  = std::ref(core::ServiceLocator<fs::VirtualFileSystem>::ref());
			lua["galaxy_nui"] = std::ref(core::ServiceLocator<ui::NuklearUI>::ref());
			// loader
			// fontcontext
			lua["galaxy_entitymeta"] = std::ref(core::ServiceLocator<meta::EntityMeta>::ref());
			// lua state
			lua["galaxy_soundengine"] = std::ref(core::ServiceLocator<media::SoundEngine>::ref());
			lua["galaxy_voiceengine"] = std::ref(core::ServiceLocator<media::VoiceEngine>::ref());
			lua["galaxy_musicengine"] = std::ref(core::ServiceLocator<media::MusicEngine>::ref());
			lua["galaxy_sounds"]      = std::ref(core::ServiceLocator<resource::SoundCache>::ref());
			lua["galaxy_music"]       = std::ref(core::ServiceLocator<resource::MusicCache>::ref());
			lua["galaxy_voice"]       = std::ref(core::ServiceLocator<resource::VoiceCache>::ref());
			lua["galaxy_videos"]      = std::ref(core::ServiceLocator<resource::VideoCache>::ref());
			lua["galaxy_shaders"]     = std::ref(core::ServiceLocator<resource::Shaders>::ref());
			lua["galaxy_fonts"]       = std::ref(core::ServiceLocator<resource::Fonts>::ref());
			lua["galaxy_textures"]    = std::ref(core::ServiceLocator<resource::Textures>::ref());
			lua["galaxy_prefabs"]     = std::ref(core::ServiceLocator<resource::Prefabs>::ref());
			lua["galaxy_scripts"]     = std::ref(core::ServiceLocator<resource::Scripts>::ref());
			lua["galaxy_dispatcher"]  = std::ref(core::ServiceLocator<entt::dispatcher>::ref());
			lua["galaxy_scenes"]      = std::ref(core::ServiceLocator<scene::SceneManager>::ref());
			lua["galaxy_fontcontext"] = std::ref(core::ServiceLocator<graphics::FontContext>::ref());
		}
	} // namespace lua
} // namespace galaxy
