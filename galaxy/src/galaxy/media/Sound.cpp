///
/// Sound.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/media/AudioEngine.hpp"

#include "Sound.hpp"

namespace galaxy
{
	namespace media
	{
		Sound::Sound()
			: m_sound {}
			, m_decoder {}
		{
		}

		Sound::Sound(SoundType type, const std::string& file)
			: m_sound {}
			, m_decoder {}
		{
			load(type, file);
		}

		Sound::~Sound()
		{
			set_looping(false);
			stop();

			ma_sound_uninit(&m_sound);
			ma_decoder_uninit(&m_decoder);
		}

		void Sound::load(SoundType type, const std::string& file)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			ma_engine*   engine = nullptr;
			unsigned int flags  = MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_NO_SPATIALIZATION;

			switch (type)
			{
				case SoundType::SFX:
					engine = core::ServiceLocator<media::SoundEngine>::ref().get_engine();
					break;

				case SoundType::MUSIC:
					engine  = core::ServiceLocator<media::MusicEngine>::ref().get_engine();
					flags  |= MA_SOUND_FLAG_STREAM;
					break;

				case SoundType::VOICE:
					engine  = core::ServiceLocator<media::VoiceEngine>::ref().get_engine();
					flags  |= MA_SOUND_FLAG_STREAM;
					break;
			}

			ma_sound_uninit(&m_sound);
			ma_decoder_uninit(&m_decoder);
			m_data.clear();

			m_data = fs.read<meta::FSBinaryR>(file);
			if (!m_data.empty())
			{
				if (ma_decoder_init_memory(m_data.data(), m_data.size(), nullptr, &m_decoder) != MA_SUCCESS)
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to read '{0}' into decoder.", file);
					ma_decoder_uninit(&m_decoder);
				}

				if (ma_sound_init_from_data_source(engine, &m_decoder, flags, nullptr, &m_sound) != MA_SUCCESS)
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to read '{0}' from decoder.", file);
					ma_sound_uninit(&m_sound);
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to read '{0}' from vfs", file);
			}
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

		void Sound::fade_in(const ma_uint64 ms)
		{
			ma_sound_set_fade_in_milliseconds(&m_sound, 0.0f, 1.0f, ms);
		}

		void Sound::fade_out(const ma_uint64 ms)
		{
			ma_sound_set_fade_in_milliseconds(&m_sound, -1.0f, 0.0f, ms);
		}

		void Sound::on_finish(ma_sound_end_proc callback, void* user_data)
		{
			ma_sound_set_end_callback(&m_sound, callback, user_data);
		}

		void Sound::set_looping(const bool loop)
		{
			ma_sound_set_looping(&m_sound, loop);
		}

		void Sound::set_volume(const float volume)
		{
			ma_sound_set_volume(&m_sound, volume);
		}

		void Sound::set_pan(const float pan)
		{
			ma_sound_set_pan(&m_sound, std::clamp(pan, -1.0f, 1.0f));
		}

		void Sound::set_pan_mode(ma_pan_mode mode)
		{
			ma_sound_set_pan_mode(&m_sound, mode);
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

		void Sound::set_spatialization_enabled(const bool enable)
		{
			ma_sound_set_spatialization_enabled(&m_sound, enable);
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

		void Sound::set_directional_attenuation_factor(const float factor)
		{
			ma_sound_set_directional_attenuation_factor(&m_sound, factor);
		}

		void Sound::set_start_time_in_milliseconds(const ma_uint64 ms)
		{
			ma_sound_set_start_time_in_milliseconds(&m_sound, ms);
		}

		void Sound::set_stop_time_in_milliseconds(const ma_uint64 ms)
		{
			ma_sound_set_stop_time_in_milliseconds(&m_sound, ms);
		}

		float Sound::get_volume() const
		{
			return ma_sound_get_volume(&m_sound);
		}

		float Sound::get_pan() const
		{
			return ma_sound_get_pan(&m_sound);
		}

		ma_pan_mode Sound::get_pan_mode() const
		{
			return ma_sound_get_pan_mode(&m_sound);
		}

		float Sound::get_pitch() const
		{
			return ma_sound_get_pitch(&m_sound);
		}

		ma_vec3f Sound::get_position() const
		{
			return ma_sound_get_position(&m_sound);
		}

		ma_vec3f Sound::get_direction() const
		{
			return ma_sound_get_direction(&m_sound);
		}

		ma_vec3f Sound::get_velocity() const
		{
			return ma_sound_get_velocity(&m_sound);
		}

		void Sound::get_cone(float* inner_angle, float* outer_angle, float* outer_gain) const
		{
			ma_sound_get_cone(&m_sound, inner_angle, outer_angle, outer_gain);
		}

		ma_attenuation_model Sound::get_attenuation_model() const
		{
			return ma_sound_get_attenuation_model(&m_sound);
		}

		ma_positioning Sound::get_positioning() const
		{
			return ma_sound_get_positioning(&m_sound);
		}

		unsigned int Sound::get_pinned_listener_index() const
		{
			return ma_sound_get_pinned_listener_index(&m_sound);
		}

		unsigned int Sound::get_listener_index() const
		{
			return ma_sound_get_listener_index(&m_sound);
		}

		ma_vec3f Sound::get_direction_to_listener() const
		{
			return ma_sound_get_direction_to_listener(&m_sound);
		}

		float Sound::get_rolloff() const
		{
			return ma_sound_get_rolloff(&m_sound);
		}

		float Sound::get_min_gain() const
		{
			return ma_sound_get_min_gain(&m_sound);
		}

		float Sound::get_max_gain() const
		{
			return ma_sound_get_max_gain(&m_sound);
		}

		float Sound::get_min_distance() const
		{
			return ma_sound_get_min_distance(&m_sound);
		}

		float Sound::get_doppler_factor() const
		{
			return ma_sound_get_doppler_factor(&m_sound);
		}

		float Sound::get_max_distance() const
		{
			return ma_sound_get_max_distance(&m_sound);
		}

		float Sound::get_directional_attenuation_factor() const
		{
			return ma_sound_get_directional_attenuation_factor(&m_sound);
		}

		float Sound::get_current_fade_volume() const
		{
			return ma_sound_get_current_fade_volume(&m_sound);
		}

		float Sound::get_cursor_in_seconds()
		{
			float cursor = 0.0f;
			ma_sound_get_cursor_in_seconds(&m_sound, &cursor);

			return cursor;
		}

		float Sound::get_length_in_seconds()
		{
			float length = 0.0f;
			ma_sound_get_length_in_seconds(&m_sound, &length);

			return length;
		}

		bool Sound::is_spatialization_enabled() const
		{
			return ma_sound_is_spatialization_enabled(&m_sound);
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
	} // namespace media
} // namespace galaxy
