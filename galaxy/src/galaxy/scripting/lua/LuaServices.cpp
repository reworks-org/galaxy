///
/// LuaServices.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/signal/dispatcher.hpp>
#include <entt/locator/locator.hpp>
#include <sol/sol.hpp>

#include "galaxy/core/Config.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/scene/SceneManager.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_services()
		{
			auto& lua = entt::locator<sol::state>::value();

			// logging
			// thread_pool
			lua["galaxy_config"] = std::ref(entt::locator<core::Config>::value());
			lua["galaxy_fs"]     = std::ref(entt::locator<fs::VirtualFileSystem>::value());
			// render window
			lua["galaxy_dispatcher"] = std::ref(entt::locator<entt::dispatcher>::value());
			// system factory
			// entity factory
			// lua state
			lua["galaxy_scenes"] = std::ref(entt::locator<scene::SceneManager>::value());

			/*lua["galaxy_nui"]         = std::ref(entt::locator<ui::NuklearUI>::value());
			lua["galaxy_entitymeta"]  = std::ref(entt::locator<meta::EntityMeta>::value());
			lua["galaxy_soundengine"] = std::ref(entt::locator<media::SoundEngine>::value());
			lua["galaxy_voiceengine"] = std::ref(entt::locator<media::VoiceEngine>::value());
			lua["galaxy_musicengine"] = std::ref(entt::locator<media::MusicEngine>::value());
			lua["galaxy_animations"]  = std::ref(entt::locator<resource::Animations>::value());
			lua["galaxy_sounds"]      = std::ref(entt::locator<resource::SoundCache>::value());
			lua["galaxy_music"]       = std::ref(entt::locator<resource::MusicCache>::value());
			lua["galaxy_voice"]       = std::ref(entt::locator<resource::VoiceCache>::value());
			lua["galaxy_videos"]      = std::ref(entt::locator<resource::VideoCache>::value());
			lua["galaxy_shaders"]     = std::ref(entt::locator<resource::Shaders>::value());
			lua["galaxy_fonts"]       = std::ref(entt::locator<resource::Fonts>::value());
			lua["galaxy_textures"]    = std::ref(entt::locator<resource::Textures>::value());
			lua["galaxy_prefabs"]     = std::ref(entt::locator<resource::Prefabs>::value());
			lua["galaxy_scripts"]     = std::ref(entt::locator<resource::Scripts>::value());
			lua["galaxy_fontcontext"] = std::ref(entt::locator<graphics::FontContext>::value());*/
		}
	} // namespace lua
} // namespace galaxy
