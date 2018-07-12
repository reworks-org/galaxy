///
/// Sound.cpp
/// starlight
///
/// Created by reworks on 12/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <allegro5/allegro_audio.h>

#include "sl/core/World.hpp"
#include "sl/libs/sol2/sol.hpp"
#include "sl/libs/loguru/loguru.hpp"
#include "sl/core/ServiceLocator.hpp"

#include "Sound.hpp"

namespace sl
{
	Sound::Sound(const sol::table& table)
	{
		// Read the lua table to get the sample file name.
		// Then load it into allegro and check if it was created.
		std::string file = Locator::world->m_soundFolderPath + table.get<std::string>("file");
		m_sound = al_load_sample(file.c_str());
		if (!m_sound)
		{
			LOG_S(FATAL) << "Unable to find sound file: " << file;
		}

		// Retrieve sample values from lua table.
		// volume (gain) - relative volume at which the sample is played; 1.0 is normal.
		// pan - 0.0 is centred, -1.0 is left, 1.0 is right, or ALLEGRO_AUDIO_PAN_NONE.
		// speed - relative speed at which the sample is played; 1.0 is normal.
		m_pan = table.get<float>("pan");
		m_speed = table.get<float>("speed");
		m_volume = table.get<float>("volume");
	}

	Sound::~Sound()
	{
		// Destroy sample when object is destroyed.
		// To prevent memory leaks.
		al_destroy_sample(m_sound);
	}

	void Sound::play()
	{
		// Play the sample once.
		al_play_sample(m_sound, m_volume, m_pan, m_speed, ALLEGRO_PLAYMODE_ONCE, nullptr);
	}

	void Sound::setPan(const float pan)
	{
		// Correct the pan and set it.
		if (pan > 1.0f)
		{
			m_pan = 1.0f;
		}
		else if (pan < -1.0f)
		{
			m_pan = -1.0f;
		}
		else
		{
			m_pan = pan;
		}
	}

	void Sound::setSpeed(const float speed)
	{
		// Correct the speed and set it.
		if (speed > 1.0f)
		{
			m_speed = 1.0f;
		}
		else if (speed < 0.0f)
		{
			m_speed = 0.0f;
		}
		else
		{
			m_speed = speed;
		}
	}

	void Sound::setVolume(const float volume)
	{
		// Correct the volume and set it.
		if (volume > 1.0f)
		{
			m_volume = 1.0f;
		}
		else if (volume < 0.0f)
		{
			m_volume = 0.0f;
		}
		else
		{
			m_volume = volume;
		}
	}
}