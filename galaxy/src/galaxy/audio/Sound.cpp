///
/// Sound.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/audio/AudioEngine.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Sound.hpp"

namespace galaxy
{
	namespace audio
	{
		Sound::Sound()
			: m_sound {}
		{
		}

		Sound::Sound(Sound::Type type, std::string_view file)
			: m_sound {}
		{
			load(type, file);
		}

		Sound::~Sound()
		{
			set_looping(false);
			stop();

			ma_sound_uninit(&m_sound);
		}

		void Sound::load(Sound::Type type, std::string_view file)
		{
			auto& ae = core::ServiceLocator<audio::AudioEngine>::ref();
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			ma_engine* engine  = nullptr;
			unsigned int flags = MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_NO_SPATIALIZATION;

			switch (type)
			{
				case Sound::Type::SFX:
					engine = ae.get_sfx_engine();
					break;

				case Sound::Type::MUSIC:
					engine = ae.get_music_engine();
					flags |= MA_SOUND_FLAG_STREAM;
					break;

				case Sound::Type::DIALOGUE:
					engine = ae.get_dialogue_engine();
					flags |= MA_SOUND_FLAG_STREAM;
					break;
			}

			auto info = fs.find(file);
			if (info.code == fs::FileCode::FOUND)
			{
				const auto result = ma_sound_init_from_file(engine, info.string.c_str(), flags, nullptr, nullptr, &m_sound);
				if (result != MA_SUCCESS)
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to load {0} from file.");
				}
			}
			else
			{
				const auto name = magic_enum::enum_name(info.code);
				GALAXY_LOG(GALAXY_ERROR, "Failed to find {0} because {1}.", file, name);
			}
		}

		void Sound::toggle_spatialization(const bool enable)
		{
			ma_sound_set_spatialization_enabled(&m_sound, enable);
		}

		void Sound::play()
		{
			ma_sound_start(&m_sound);
		}

		void Sound::pause()
		{
			ma_sound_stop(&m_sound);
		}

		void Sound::stop()
		{
			ma_sound_stop(&m_sound);
			ma_sound_seek_to_pcm_frame(&m_sound, 0);
		}

		void Sound::set_looping(const bool loop)
		{
			ma_sound_set_looping(&m_sound, loop);
		}

		void Sound::set_volume(const float volume)
		{
			ma_sound_set_volume(&m_sound, volume);
		}

		void Sound::fade_in(const ma_uint64 ms)
		{
			ma_sound_set_fade_in_milliseconds(&m_sound, 0.0f, 1.0f, ms);
		}

		void Sound::fade_out(const ma_uint64 ms)
		{
			ma_sound_set_fade_in_milliseconds(&m_sound, -1.0f, 0.0f, ms);
		}

		void Sound::set_pan(const float pan)
		{
			ma_sound_set_pan(&m_sound, std::clamp(pan, -1.0f, 1.0f));
		}

		void Sound::set_pitch(const float pitch)
		{
			ma_sound_set_pitch(&m_sound, std::max(0.1f, pitch));
		}

		void Sound::set_pinned_listener(const unsigned int id)
		{
			ma_sound_set_pinned_listener_index(&m_sound, id);
		}

		void Sound::set_positioning(ma_positioning positioning)
		{
			ma_sound_set_positioning(&m_sound, positioning);
		}

		void Sound::set_position(const float x, const float y, const float z)
		{
			ma_sound_set_position(&m_sound, x, y, z);
		}

		void Sound::set_direction(const float x, const float y, const float z)
		{
			ma_sound_set_direction(&m_sound, x, y, z);
		}

		void Sound::set_cone(float inner_angle, const float outer_angle, const float outer_gain)
		{
			ma_sound_set_cone(&m_sound, inner_angle, outer_angle, outer_gain);
		}

		void Sound::set_velocity(const float x, const float y, const float z)
		{
			ma_sound_set_velocity(&m_sound, x, y, z);
		}

		void Sound::set_attenuation_model(ma_attenuation_model model)
		{
			ma_sound_set_attenuation_model(&m_sound, model);
		}

		void Sound::set_rolloff(const float rolloff)
		{
			ma_sound_set_rolloff(&m_sound, rolloff);
		}

		void Sound::set_min_gain(const float gain)
		{
			ma_sound_set_min_gain(&m_sound, gain);
		}

		void Sound::set_max_gain(const float gain)
		{
			ma_sound_set_max_gain(&m_sound, gain);
		}

		void Sound::set_min_distance(const float distance)
		{
			ma_sound_set_min_distance(&m_sound, distance);
		}

		void Sound::set_max_distance(const float distance)
		{
			ma_sound_set_max_distance(&m_sound, distance);
		}

		void Sound::set_doppler_factor(const float factor)
		{
			ma_sound_set_doppler_factor(&m_sound, factor);
		}

		bool Sound::is_playing() const
		{
			return ma_sound_is_playing(&m_sound);
		}

		bool Sound::is_looping() const
		{
			return ma_sound_is_looping(&m_sound);
		}

		bool Sound::is_finished() const
		{
			return ma_sound_at_end(&m_sound);
		}
	} // namespace audio
} // namespace galaxy