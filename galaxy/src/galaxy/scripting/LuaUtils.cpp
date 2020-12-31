///
/// LuaUtils.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include "galaxy/algorithm/Algorithm.hpp"
#include "galaxy/algorithm/Random.hpp"

#include "galaxy/audio/Playlist.hpp"

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/World.hpp"

#include "LuaUtils.hpp"

namespace galaxy
{
	namespace lua
	{
		void register_functions()
		{
			auto lua = SL_HANDLE.lua();

			lua->set_function("random_int", &algorithm::random<int>);
			lua->set_function("random_float", &algorithm::random<float>);
			lua->set_function("normalize", &algorithm::normalize<float>);
		}

		void register_audio()
		{
			auto lua = SL_HANDLE.lua();

			auto music_type           = lua->new_usertype<audio::Music>("Music", sol::constructors<audio::Music()>());
			music_type["load"]        = &audio::Music::load;
			music_type["play"]        = &audio::Music::play;
			music_type["pause"]       = &audio::Music::pause;
			music_type["stop"]        = &audio::Music::stop;
			music_type["rewind"]      = &audio::Music::rewind;
			music_type["set_looping"] = &audio::Music::set_looping;

			auto sound_type           = lua->new_usertype<audio::Sound>("Sound", sol::constructors<audio::Sound()>());
			sound_type["load"]        = &audio::Sound::load;
			sound_type["play"]        = &audio::Sound::play;
			sound_type["pause"]       = &audio::Sound::pause;
			sound_type["stop"]        = &audio::Sound::stop;
			sound_type["rewind"]      = &audio::Sound::rewind;
			sound_type["set_looping"] = &audio::Sound::set_looping;

			auto music_playlist_type           = lua->new_usertype<audio::Playlist<audio::Music>>("MusicPlaylist", sol::constructors<audio::Playlist<audio::Music>()>());
			music_playlist_type["load"]        = &audio::Playlist<audio::Music>::add;
			music_playlist_type["play"]        = &audio::Playlist<audio::Music>::play;
			music_playlist_type["pause"]       = &audio::Playlist<audio::Music>::pause;
			music_playlist_type["stop"]        = &audio::Playlist<audio::Music>::stop;
			music_playlist_type["rewind"]      = &audio::Playlist<audio::Music>::shuffle;
			music_playlist_type["set_looping"] = &audio::Playlist<audio::Music>::toggle_loop;

			auto sound_playlist_type           = lua->new_usertype<audio::Playlist<audio::Sound>>("SoundPlaylist", sol::constructors<audio::Playlist<audio::Sound>()>());
			sound_playlist_type["load"]        = &audio::Playlist<audio::Sound>::add;
			sound_playlist_type["play"]        = &audio::Playlist<audio::Sound>::play;
			sound_playlist_type["pause"]       = &audio::Playlist<audio::Sound>::pause;
			sound_playlist_type["stop"]        = &audio::Playlist<audio::Sound>::stop;
			sound_playlist_type["rewind"]      = &audio::Playlist<audio::Sound>::shuffle;
			sound_playlist_type["set_looping"] = &audio::Playlist<audio::Sound>::toggle_loop;
		}
	} // namespace lua
} // namespace galaxy