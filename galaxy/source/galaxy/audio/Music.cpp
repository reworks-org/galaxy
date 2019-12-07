///
/// Music.cpp
/// galaxy
///
/// Created by reworks on 13/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <allegro5/allegro_audio.h>

#include "galaxy/core/World.hpp"
#include "galaxy/core/ServiceLocator.hpp"

#include "Music.hpp"

namespace galaxy
{
	Music::Music(const std::string& file, const ALLEGRO_PLAYMODE playmode, const int pan, const int speed, const int volume)
		:m_position(0), m_music(nullptr), m_instance(nullptr)
	{
		// Read the lua table to get the sample file name.
		// Then load it into allegro and check if it was created.
		std::string fullPath = Locator::world->m_musicFolderPath + file;
		m_music = al_load_sample(fullPath.c_str());
		if (!m_music)
		{
			LOG_S(FATAL) << "Failed to load music file: " << file << ". Errno: " << al_get_errno();
		}

		// Then create an instance of the music and check if it was created.
		m_instance = al_create_sample_instance(m_music);
		if (!m_instance)
		{
			LOG_S(FATAL) << "Failed to create instance: " << file << ". Errno: " << al_get_errno();
		}

		// Set the playmode.
		// 0 - Play once.
		// 1 - Loop song.
		// 2 - Bi-directional.
		switch (playmode)
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
			LOG_S(WARNING) << "Invalid playmode " << playmode << " for " << file << ". Setting to loop. Errno: " << al_get_errno();
			al_set_sample_instance_playmode(m_instance, ALLEGRO_PLAYMODE_LOOP);
			break;
		}

		if (!setPan(pan))
		{
			LOG_S(ERROR) << "Failed to set music pan. Errno: " << al_get_errno();
		}

		if (!setSpeed(speed))
		{
			LOG_S(ERROR) << "Failed to set music speed. Errno: " << al_get_errno();
		}

		if (!setVolume(volume))
		{
			LOG_S(ERROR) << "Failed to set music volume. Errno: " << al_get_errno();
		}


		// Attach to allegro's mixer.
		if (!al_attach_sample_instance_to_mixer(m_instance, al_get_default_mixer()))
		{
			LOG_S(FATAL) << "Failed to attach sample instance to mixer. Errno: " << al_get_errno();
		}
	}

	Music::Music(const sol::table& table)
		:m_position(0), m_music(nullptr), m_instance(nullptr)
	{
		// Read the lua table to get the sample file name.
		// Then load it into allegro and check if it was created.
		std::string file = Locator::world->m_musicFolderPath + table.get<std::string>("file");
		m_music = al_load_sample(file.c_str());
		if (!m_music)
		{
			LOG_S(FATAL) << "Failed to load music file: " << file << ". Errno: " << al_get_errno();
		}

		// Then create an instance of the music and check if it was created.
		m_instance = al_create_sample_instance(m_music);
		if (!m_instance)
		{
			LOG_S(FATAL) << "Failed to create instance: " << file << ". Errno: " << al_get_errno();
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
				LOG_S(WARNING) << "Invalid playmode " << table.get<int>("playmode") << " for " << file << ". Setting to loop. Errno: " << al_get_errno();
				al_set_sample_instance_playmode(m_instance, ALLEGRO_PLAYMODE_LOOP);
				break;
		}
		
		if (!setPan(table.get<float>("pan")))
		{
			LOG_S(ERROR) << "Failed to set music pan. Errno: " << al_get_errno();
		}
		
		if (!setSpeed(table.get<float>("speed")))
		{
			LOG_S(ERROR) << "Failed to set music speed. Errno: " << al_get_errno();
		}

		if (!setVolume(table.get<float>("volume")))
		{
			LOG_S(ERROR) << "Failed to set music volume. Errno: " << al_get_errno();
		}

		// Attach to allegro's mixer.
		if (!al_attach_sample_instance_to_mixer(m_instance, al_get_default_mixer()))
		{
			LOG_S(FATAL) << "Failed to attach sample instance to mixer. Errno: " << al_get_errno();
		}
	}

	Music::~Music()
	{
		// Ensure music is stopped before destroying it.
		stop();

		// Destroy instance and music when object is destroyed.
		if (m_instance)
		{
			al_destroy_sample_instance(m_instance);
		}
		
		if (m_music)
		{
			al_destroy_sample(m_music);
		}
	}

	void Music::play() const
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

	void Music::resume() const
	{
		// Resume from stopping point.
		al_set_sample_instance_position(m_instance, m_position);
		al_play_sample_instance(m_instance);
	}

	bool Music::setPan(const float pan)
	{
		// Clamp the pan and set it.
		return al_set_sample_instance_pan(m_instance, std::clamp(pan, -1.0f, 1.0f));
	}

	bool Music::setSpeed(const float speed)
	{
		// Check the speed and set it.
		return al_set_sample_instance_speed(m_instance, std::max(0.1f, speed));
	}

	bool Music::setVolume(const float volume)
	{
		// Check the volume and set it.
		return al_set_sample_instance_gain(m_instance, std::max(0.0f, volume));
	}
}