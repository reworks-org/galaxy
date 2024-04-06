///
/// LuaResource.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/resource/Animations.hpp"
#include "galaxy/resource/Fonts.hpp"
#include "galaxy/resource/Media.hpp"
#include "galaxy/resource/Prefabs.hpp"
#include "galaxy/resource/Scripts.hpp"
#include "galaxy/resource/Shaders.hpp"
#include "galaxy/resource/Textures.hpp"

#include "Lua.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_resources()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();

			auto anims_type           = lua.new_usertype<resource::Animations>("Animations", sol::no_constructor);
			anims_type["clear"]       = &resource::Animations::clear;
			anims_type["empty"]       = &resource::Animations::empty;
			anims_type["get"]         = &resource::Animations::get;
			anims_type["has"]         = &resource::Animations::has;
			anims_type["keys"]        = &resource::Animations::keys;
			anims_type["size"]        = &resource::Animations::size;
			anims_type["insert"]      = &resource::Animations::insert;
			anims_type["load"]        = &resource::Animations::load;
			anims_type["load_folder"] = &resource::Animations::load_folder;

			auto fonts_type           = lua.new_usertype<resource::Fonts>("Fonts", sol::no_constructor);
			fonts_type["clear"]       = &resource::Fonts::clear;
			fonts_type["empty"]       = &resource::Fonts::empty;
			fonts_type["get"]         = &resource::Fonts::get;
			fonts_type["has"]         = &resource::Fonts::has;
			fonts_type["keys"]        = &resource::Fonts::keys;
			fonts_type["size"]        = &resource::Fonts::size;
			fonts_type["insert"]      = &resource::Fonts::insert;
			fonts_type["load"]        = &resource::Fonts::load;
			fonts_type["load_folder"] = &resource::Fonts::load_folder;

			auto soundCache_type           = lua.new_usertype<resource::SoundCache>("SoundCaches", sol::no_constructor);
			soundCache_type["clear"]       = &resource::SoundCache::clear;
			soundCache_type["empty"]       = &resource::SoundCache::empty;
			soundCache_type["get"]         = &resource::SoundCache::get;
			soundCache_type["has"]         = &resource::SoundCache::has;
			soundCache_type["keys"]        = &resource::SoundCache::keys;
			soundCache_type["size"]        = &resource::SoundCache::size;
			soundCache_type["insert"]      = &resource::SoundCache::insert;
			soundCache_type["load"]        = &resource::SoundCache::load;
			soundCache_type["load_folder"] = &resource::SoundCache::load_folder;

			auto musiccache_type           = lua.new_usertype<resource::MusicCache>("MusicCache", sol::no_constructor);
			musiccache_type["clear"]       = &resource::MusicCache::clear;
			musiccache_type["empty"]       = &resource::MusicCache::empty;
			musiccache_type["get"]         = &resource::MusicCache::get;
			musiccache_type["has"]         = &resource::MusicCache::has;
			musiccache_type["keys"]        = &resource::MusicCache::keys;
			musiccache_type["size"]        = &resource::MusicCache::size;
			musiccache_type["insert"]      = &resource::MusicCache::insert;
			musiccache_type["load"]        = &resource::MusicCache::load;
			musiccache_type["load_folder"] = &resource::MusicCache::load_folder;

			auto voicecache_type           = lua.new_usertype<resource::VoiceCache>("VoiceCache", sol::no_constructor);
			voicecache_type["clear"]       = &resource::VoiceCache::clear;
			voicecache_type["empty"]       = &resource::VoiceCache::empty;
			voicecache_type["get"]         = &resource::VoiceCache::get;
			voicecache_type["has"]         = &resource::VoiceCache::has;
			voicecache_type["keys"]        = &resource::VoiceCache::keys;
			voicecache_type["size"]        = &resource::VoiceCache::size;
			voicecache_type["insert"]      = &resource::VoiceCache::insert;
			voicecache_type["load"]        = &resource::VoiceCache::load;
			voicecache_type["load_folder"] = &resource::VoiceCache::load_folder;

			auto videocache_type           = lua.new_usertype<resource::VideoCache>("VideoCache", sol::no_constructor);
			videocache_type["clear"]       = &resource::VideoCache::clear;
			videocache_type["empty"]       = &resource::VideoCache::empty;
			videocache_type["get"]         = &resource::VideoCache::get;
			videocache_type["has"]         = &resource::VideoCache::has;
			videocache_type["keys"]        = &resource::VideoCache::keys;
			videocache_type["size"]        = &resource::VideoCache::size;
			videocache_type["insert"]      = &resource::VideoCache::insert;
			videocache_type["load"]        = &resource::VideoCache::load;
			videocache_type["load_folder"] = &resource::VideoCache::load_folder;

			auto prefabs_type           = lua.new_usertype<resource::Prefabs>("Prefabs", sol::no_constructor);
			prefabs_type["clear"]       = &resource::Prefabs::clear;
			prefabs_type["empty"]       = &resource::Prefabs::empty;
			prefabs_type["get"]         = &resource::Prefabs::get;
			prefabs_type["has"]         = &resource::Prefabs::has;
			prefabs_type["keys"]        = &resource::Prefabs::keys;
			prefabs_type["size"]        = &resource::Prefabs::size;
			prefabs_type["insert"]      = &resource::Prefabs::insert;
			prefabs_type["load"]        = &resource::Prefabs::load;
			prefabs_type["load_folder"] = &resource::Prefabs::load_folder;

			auto scripts_type           = lua.new_usertype<resource::Scripts>("Scripts", sol::no_constructor);
			scripts_type["clear"]       = &resource::Scripts::clear;
			scripts_type["empty"]       = &resource::Scripts::empty;
			scripts_type["get"]         = &resource::Scripts::get;
			scripts_type["has"]         = &resource::Scripts::has;
			scripts_type["keys"]        = &resource::Scripts::keys;
			scripts_type["size"]        = &resource::Scripts::size;
			scripts_type["insert"]      = &resource::Scripts::insert;
			scripts_type["load"]        = &resource::Scripts::load;
			scripts_type["load_folder"] = &resource::Scripts::load_folder;

			auto shaders_type           = lua.new_usertype<resource::Shaders>("Shaders", sol::no_constructor);
			shaders_type["clear"]       = &resource::Shaders::clear;
			shaders_type["empty"]       = &resource::Shaders::empty;
			shaders_type["get"]         = &resource::Shaders::get;
			shaders_type["has"]         = &resource::Shaders::has;
			shaders_type["keys"]        = &resource::Shaders::keys;
			shaders_type["size"]        = &resource::Shaders::size;
			shaders_type["insert"]      = &resource::Shaders::insert;
			shaders_type["load"]        = &resource::Shaders::load;
			shaders_type["load_folder"] = &resource::Shaders::load_folder;

			auto textures_type           = lua.new_usertype<resource::Textures>("Textures", sol::no_constructor);
			textures_type["clear"]       = &resource::Textures::clear;
			textures_type["empty"]       = &resource::Textures::empty;
			textures_type["get"]         = &resource::Textures::get;
			textures_type["has"]         = &resource::Textures::has;
			textures_type["keys"]        = &resource::Textures::keys;
			textures_type["size"]        = &resource::Textures::size;
			textures_type["insert"]      = &resource::Textures::insert;
			textures_type["load"]        = &resource::Textures::load;
			textures_type["load_folder"] = &resource::Textures::load_folder;
		}
	} // namespace lua
} // namespace galaxy
