///
/// Music.cpp
/// starlight
///
/// Created by reworks on 13/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <allegro5/allegro_audio.h>

#include "sl/fs/VirtualFS.hpp"
#include "sl/libs/sol2/sol.hpp"
#include "sl/libs/loguru/loguru.hpp"

#include "Music.hpp"

namespace sl
{
	Music::Music(const sol::table& table)
		:m_position(0), m_music(nullptr), m_instance(nullptr)
	{
		// Read the lua table to get the sample file name.
		// Then load it into allegro and check if it was created.
		const char* file = table.get<const char*>("file");
		m_music = al_load_sample(file);
		if (!m_music)
		{
			LOG_S(FATAL) << "Failed to load music file: " << file;
		}

		// Then create an instance of the music and check if it was created.
		m_instance = al_create_sample_instance(m_music);
		if (!m_instance)
		{
			LOG_S(FATAL) << "Failed to create instance: " << file;
		}

		// Set the playmode.
		// 0 - Play once.
		// 1 - Loop song.
		// 2 - Bi-directional.
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
				LOG_S(WARNING) << "Invalid playmode for " << file << ". Setting to loop...";
				al_set_sample_instance_playmode(m_instance, ALLEGRO_PLAYMODE_LOOP);
				break;
		}
		
		// Volume (gain) - relative volume at which the sample is played; 1.0 is normal.
		// Pan - 0.0 is centred, -1.0 is left, 1.0 is right, or ALLEGRO_AUDIO_PAN_NONE.
		// Speed - relative speed at which the sample is played; 1.0 is normal.
		al_set_sample_instance_pan(m_instance, table.get<float>("pan"));
		al_set_sample_instance_speed(m_instance, table.get<float>("speed"));
		al_set_sample_instance_gain(m_instance, table.get<float>("volume"));

		// Attach to allegro's mixer.
		al_attach_sample_instance_to_mixer(m_instance, al_get_default_mixer());
	}

	Music::~Music()
	{
		stop();

		// Destroy instance and music when object is destroyed.
		al_destroy_sample_instance(m_instance);
		al_destroy_sample(m_music);
	}

	void Music::play()
	{
		// Play song from beginning.
		al_set_sample_instance_position(m_instance, 0);
		al_play_sample_instance(m_instance);
	}

	void Music::stop()
	{
		// Stop song.
		m_position = al_get_sample_instance_position(m_instance);
		al_stop_sample_instance(m_instance);
	}

	void Music::resume()
	{
		// Resume from stopping point.
		al_set_sample_instance_position(m_instance, m_position);
		al_play_sample_instance(m_instance);
	}

	void Music::setPan(const float pan)
	{
		// Correct the pan and set it.
		if (pan > 1.0f)
		{
			al_set_sample_instance_pan(m_instance, 1.0f);
		}
		else if (pan < -1.0f)
		{
			al_set_sample_instance_pan(m_instance, -1.0f);
		}
		else
		{
			al_set_sample_instance_pan(m_instance, pan);
		}
	}

	void Music::setSpeed(const float speed)
	{
		// Correct the speed and set it.
		if (speed > 1.0f)
		{
			al_set_sample_instance_speed(m_instance, 1.0f);
		}
		else if (speed < 0.0f)
		{
			al_set_sample_instance_speed(m_instance, 0.0f);
		}
		else
		{
			al_set_sample_instance_speed(m_instance, speed);
		}
	}

	void Music::setVolume(const float volume)
	{
		// Correct the volume and set it.
		if (volume > 1.0f)
		{
			al_set_sample_instance_gain(m_instance, 1.0f);
		}
		else if (volume < 0.0f)
		{
			al_set_sample_instance_gain(m_instance, 0.0f);
		}
		else
		{
			al_set_sample_instance_gain(m_instance, volume);
		}
	}
}