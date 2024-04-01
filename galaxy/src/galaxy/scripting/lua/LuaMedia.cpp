///
/// LuaMedia.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/media/AudioEngine.hpp"
#include "galaxy/media/Sound.hpp"
#include "galaxy/media/Video.hpp"

#include "Lua.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_media()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();

			// clang-format off
			lua.new_enum<media::SoundType>("SoundType",
			{
				{"VOICE", media::SoundType::VOICE},
				{"MUSIC", media::SoundType::MUSIC},
				{"SFX", media::SoundType::SFX}
			});
			// clang-format on

			auto soundengine_type                      = lua.new_usertype<media::SoundEngine>("SoundEngine", sol::no_constructor);
			soundengine_type["start_device"]           = &media::SoundEngine::start_device;
			soundengine_type["stop_device"]            = &media::SoundEngine::stop_device;
			soundengine_type["stop"]                   = &media::SoundEngine::stop;
			soundengine_type["set_listener_position"]  = &media::SoundEngine::set_listener_position;
			soundengine_type["set_listener_velocity"]  = &media::SoundEngine::set_listener_velocity;
			soundengine_type["set_listener_direction"] = &media::SoundEngine::set_listener_direction;
			soundengine_type["set_listener_world_up"]  = &media::SoundEngine::set_listener_world_up;
			soundengine_type["set_listener_cone"]      = &media::SoundEngine::set_listener_cone;
			soundengine_type["set_volume"]             = &media::SoundEngine::set_volume;
			soundengine_type["toggle_listener"]        = &media::SoundEngine::toggle_listener;
			soundengine_type["is_listener_enabled"]    = &media::SoundEngine::is_listener_enabled;
			soundengine_type["find_closest_listener"]  = &media::SoundEngine::find_closest_listener;
			soundengine_type["get_listener_count"]     = &media::SoundEngine::get_listener_count;
			soundengine_type["get_samplerate"]         = &media::SoundEngine::get_samplerate;
			soundengine_type["get_channels"]           = &media::SoundEngine::get_channels;

			auto musicengine_type                      = lua.new_usertype<media::MusicEngine>("MusicEngine", sol::no_constructor);
			musicengine_type["set_listener_cone"]      = &media::MusicEngine::set_listener_cone;
			musicengine_type["set_listener_direction"] = &media::MusicEngine::set_listener_direction;
			musicengine_type["set_listener_position"]  = &media::MusicEngine::set_listener_position;
			musicengine_type["set_listener_world_up"]  = &media::MusicEngine::set_listener_world_up;
			musicengine_type["toggle_listener"]        = &media::MusicEngine::toggle_listener;
			musicengine_type["stop"]                   = &media::MusicEngine::stop;
			musicengine_type["find_closest_listener"]  = &media::MusicEngine::find_closest_listener;
			musicengine_type["get_channels"]           = &media::MusicEngine::get_channels;
			musicengine_type["get_listener_count"]     = &media::MusicEngine::get_listener_count;
			musicengine_type["get_samplerate"]         = &media::MusicEngine::get_samplerate;
			musicengine_type["is_listener_enabled"]    = &media::MusicEngine::is_listener_enabled;
			musicengine_type["set_listener_velocity"]  = &media::MusicEngine::set_listener_velocity;
			musicengine_type["set_volume"]             = &media::MusicEngine::set_volume;
			musicengine_type["start_device"]           = &media::MusicEngine::start_device;
			musicengine_type["stop_device"]            = &media::MusicEngine::stop_device;

			auto voiceengine_type                      = lua.new_usertype<media::VoiceEngine>("VoiceEngine", sol::no_constructor);
			voiceengine_type["set_listener_cone"]      = &media::VoiceEngine::set_listener_cone;
			voiceengine_type["set_listener_direction"] = &media::VoiceEngine::set_listener_direction;
			voiceengine_type["set_listener_position"]  = &media::VoiceEngine::set_listener_position;
			voiceengine_type["set_listener_world_up"]  = &media::VoiceEngine::set_listener_world_up;
			voiceengine_type["toggle_listener"]        = &media::VoiceEngine::toggle_listener;
			voiceengine_type["stop"]                   = &media::VoiceEngine::stop;
			voiceengine_type["find_closest_listener"]  = &media::VoiceEngine::find_closest_listener;
			voiceengine_type["get_channels"]           = &media::VoiceEngine::get_channels;
			voiceengine_type["get_listener_count"]     = &media::VoiceEngine::get_listener_count;
			voiceengine_type["get_samplerate"]         = &media::VoiceEngine::get_samplerate;
			voiceengine_type["is_listener_enabled"]    = &media::VoiceEngine::is_listener_enabled;
			voiceengine_type["set_listener_velocity"]  = &media::VoiceEngine::set_listener_velocity;
			voiceengine_type["set_volume"]             = &media::VoiceEngine::set_volume;
			voiceengine_type["start_device"]           = &media::VoiceEngine::start_device;
			voiceengine_type["stop_device"]            = &media::VoiceEngine::stop_device;

			auto sound_type                                  = lua.new_usertype<media::Sound>("Sound", sol::constructors<media::Sound()>());
			sound_type["fade_in"]                            = &media::Sound::fade_in;
			sound_type["fade_out"]                           = &media::Sound::fade_out;
			sound_type["is_finished"]                        = &media::Sound::is_finished;
			sound_type["is_looping"]                         = &media::Sound::is_looping;
			sound_type["is_playing"]                         = &media::Sound::is_playing;
			sound_type["load"]                               = &media::Sound::load;
			sound_type["set_attenuation_model"]              = &media::Sound::set_attenuation_model;
			sound_type["set_cone"]                           = &media::Sound::set_cone;
			sound_type["set_direction"]                      = &media::Sound::set_direction;
			sound_type["set_doppler_factor"]                 = &media::Sound::set_doppler_factor;
			sound_type["set_looping"]                        = &media::Sound::set_looping;
			sound_type["set_max_distance"]                   = &media::Sound::set_max_distance;
			sound_type["set_max_gain"]                       = &media::Sound::set_max_gain;
			sound_type["set_min_distance"]                   = &media::Sound::set_min_distance;
			sound_type["set_min_gain"]                       = &media::Sound::set_min_gain;
			sound_type["set_pan"]                            = &media::Sound::set_pan;
			sound_type["set_pinned_listener"]                = &media::Sound::set_pinned_listener;
			sound_type["set_pitch"]                          = &media::Sound::set_pitch;
			sound_type["set_position"]                       = &media::Sound::set_position;
			sound_type["set_positioning"]                    = &media::Sound::set_positioning;
			sound_type["set_rolloff"]                        = &media::Sound::set_rolloff;
			sound_type["set_velocity"]                       = &media::Sound::set_velocity;
			sound_type["set_volume"]                         = &media::Sound::set_volume;
			sound_type["play"]                               = &media::Sound::play;
			sound_type["pause"]                              = &media::Sound::pause;
			sound_type["stop"]                               = &media::Sound::stop;
			sound_type["get_attenuation_model"]              = &media::Sound::get_attenuation_model;
			sound_type["get_cone"]                           = &media::Sound::get_cone;
			sound_type["get_current_fade_volume"]            = &media::Sound::get_current_fade_volume;
			sound_type["get_cursor_in_seconds"]              = &media::Sound::get_cursor_in_seconds;
			sound_type["get_direction"]                      = &media::Sound::get_direction;
			sound_type["get_directional_attenuation_factor"] = &media::Sound::get_directional_attenuation_factor;
			sound_type["get_direction_to_listener"]          = &media::Sound::get_direction_to_listener;
			sound_type["get_doppler_factor"]                 = &media::Sound::get_doppler_factor;
			sound_type["get_length_in_seconds"]              = &media::Sound::get_length_in_seconds;
			sound_type["get_listener_index"]                 = &media::Sound::get_listener_index;
			sound_type["get_max_distance"]                   = &media::Sound::get_max_distance;
			sound_type["get_max_gain"]                       = &media::Sound::get_max_gain;
			sound_type["get_min_distance"]                   = &media::Sound::get_min_distance;
			sound_type["get_pan"]                            = &media::Sound::get_pan;
			sound_type["get_pan_mode"]                       = &media::Sound::get_pan_mode;
			sound_type["get_pinned_listener_index"]          = &media::Sound::get_pinned_listener_index;
			sound_type["get_pitch"]                          = &media::Sound::get_pitch;
			sound_type["get_position"]                       = &media::Sound::get_position;
			sound_type["get_positioning"]                    = &media::Sound::get_positioning;
			sound_type["get_rolloff"]                        = &media::Sound::get_rolloff;
			sound_type["get_velocity"]                       = &media::Sound::get_velocity;
			sound_type["get_volume"]                         = &media::Sound::get_volume;
			sound_type["is_spatialization_enabled"]          = &media::Sound::is_spatialization_enabled;
			sound_type["set_directional_attenuation_factor"] = &media::Sound::set_directional_attenuation_factor;
			sound_type["set_pan_mode"]                       = &media::Sound::set_pan_mode;
			sound_type["set_spatialization_enabled"]         = &media::Sound::set_spatialization_enabled;
			sound_type["set_start_time_in_milliseconds"]     = &media::Sound::set_start_time_in_milliseconds;
			sound_type["set_stop_time_in_milliseconds"]      = &media::Sound::set_stop_time_in_milliseconds;

			auto video_type              = lua.new_usertype<media::Video>("Video", sol::constructors<media::Video()>());
			video_type["load"]           = &media::Video::load;
			video_type["build"]          = &media::Video::build;
			video_type["update"]         = &media::Video::update;
			video_type["is_finished"]    = &media::Video::is_finished;
			video_type["get_time"]       = &media::Video::get_time;
			video_type["get_framerate"]  = &media::Video::get_framerate;
			video_type["get_samplerate"] = &media::Video::get_samplerate;
			video_type["get_duration"]   = &media::Video::get_duration;
		}
	} // namespace lua
} // namespace galaxy
