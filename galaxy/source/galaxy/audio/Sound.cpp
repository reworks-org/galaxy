///
/// Sound.cpp
/// galaxy
///
/// Created by reworks on 12/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <allegro5/allegro_audio.h>

#include "galaxy/core/World.hpp"
#include "galaxy/core/ServiceLocator.hpp"

#include "Sound.hpp"

namespace galaxy
{
	Sound::Sound(const std::string& file, const float pan, const float speed, const float volume)
		:m_pan(0), m_speed(0), m_volume(0), m_sound(nullptr)
	{
		// Read the lua table to get the sample file name.
		// Then load it into allegro and check if it was created.
		std::string fullPath = Locator::world->m_soundFolderPath + file;
		m_sound = al_load_sample(fullPath.c_str());
		if (!m_sound)
		{
			LOG_S(FATAL) << "Unable to find sound file: " << file << ". Errno: " << al_get_errno();
		}

		setPan(pan);
		setSpeed(speed);
		setVolume(volume);
	}

	Sound::Sound(const sol::table& table)
		:m_pan(0), m_speed(0), m_volume(0), m_sound(nullptr)
	{
		// Read the lua table to get the sample file name.
		// Then load it into allegro and check if it was created.
		std::string file = Locator::world->m_soundFolderPath + table.get<std::string>("file");
		m_sound = al_load_sample(file.c_str());
		if (!m_sound)
		{
			LOG_S(FATAL) << "Unable to find sound file: " << file << ". Errno: " << al_get_errno();
		}

		setPan(table.get<float>("pan"));
		setSpeed(table.get<float>("speed"));
		setVolume(table.get<float>("volume"));
	}

	Sound::~Sound()
	{
		// Destroy sample when object is destroyed.
		// To prevent memory leaks.
		if (m_sound)
		{
			al_destroy_sample(m_sound);
		}
	}

	void Sound::play() const
	{
		// Play the sample once.
		al_play_sample(m_sound, m_volume, m_pan, m_speed, ALLEGRO_PLAYMODE_ONCE, nullptr);
	}

	void Sound::setPan(const float pan)
	{
		// Clamp the pan and set it.
		m_pan = std::clamp(pan, -1.0f, 1.0f);
	}

	void Sound::setSpeed(const float speed)
	{
		// Check the speed and set it.
		m_speed = std::max(0.1f, speed);
	}

	void Sound::setVolume(const float volume)
	{
		// Check the volume and set it.
		m_volume = std::max(0.0f, volume);
	}
}