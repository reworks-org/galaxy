///
/// Music.cpp
/// starlight
///
/// Created by reworks on 13/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <string_view>
#include <allegro5/allegro_audio.h>

#include "sl/fs/VirtualFS.hpp"
#include "sl/libs/sol2/sol.hpp"
#include "sl/libs/loguru/loguru.hpp"

#include "Music.hpp"

namespace sl
{
	Music::Music(const sol::table& table)
	{
		m_music = al_load_sample(table.get<const char*>("file"));
		if (!m_music)
		{
			LOG_S(WARNING) << "Failed to load music file: " << table.get<std::string_view>("file");
		}

		m_instance = al_create_sample_instance(m_music);
		if (!m_instance)
		{
			LOG_S(WARNING) << "Failed to create instance: " << table.get<std::string_view>("file");
		}

		/// 0 - play once
		/// 1 - loop
		/// 2 - bi direcitonal
		switch (table.get<int>("playmode"))
		{
			case 0:
				al_set_sample_instance_playmode(m_instance, ALLEGRO_PLAYMODE_ONCE);
				break;

			case 1:
				al_set_sample_instance_playmode(m_instance, ALLEGRO_PLAYMODE_LOOP);
				break;

			case 2:
				al_set_sample_instance_playmode(m_instance, ALLEGRO_PLAYMODE_BIDIR);
				break;

			default:
				LOG_S(WARNING) << "Invalid playmode. Setting to loop...";
				al_set_sample_instance_playmode(m_instance, ALLEGRO_PLAYMODE_LOOP);
				break;
		}
		
		/// volume (gain) - relative volume at which the sample is played; 1.0 is normal.
		/// pan - 0.0 is centred, -1.0 is left, 1.0 is right, or ALLEGRO_AUDIO_PAN_NONE.
		/// speed - relative speed at which the sample is played; 1.0 is normal.
		al_set_sample_instance_pan(m_instance, table.get<float>("pan"));
		al_set_sample_instance_speed(m_instance, table.get<float>("speed"));
		al_set_sample_instance_gain(m_instance, table.get<float>("volume"));

		al_attach_sample_instance_to_mixer(m_instance, al_get_default_mixer());
	}

	Music::~Music()
	{
		al_destroy_sample_instance(m_instance);
		al_destroy_sample(m_music);
	}

	void Music::play()
	{
		al_set_sample_instance_position(m_instance, 0);
		al_play_sample_instance(m_instance);
	}

	void Music::stop()
	{
		m_position = al_get_sample_instance_position(m_instance);
		al_stop_sample_instance(m_instance);
	}

	void Music::resume()
	{
		al_set_sample_instance_position(m_instance, m_position);
		al_play_sample_instance(m_instance);
	}

	void Music::setPan(const float pan)
	{
		al_set_sample_instance_pan(m_instance, pan);
	}

	void Music::setSpeed(const float speed)
	{
		al_set_sample_instance_speed(m_instance, speed);
	}

	void Music::setVolume(const float volume)
	{
		al_set_sample_instance_gain(m_instance, volume);
	}
}