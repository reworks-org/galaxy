///
///  Sound.cpp
///  starlight
///
///  Created by reworks on 12/10/2017.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#include <allegro5/allegro_audio.h>

#include "sol2/sol.hpp"
#include "sl/fs/VFS.hpp"
#include "loguru/loguru.hpp"

#include "Sound.hpp"

namespace sl
{
	Sound::Sound(const sol::table& table)
	{
		m_sound = al_load_sample(table.get<std::string>("file").c_str());
		if (!m_sound)
		{
			LOG_S(WARNING) << "Unable to load sound file: " << table.get<std::string>("file");
		}

		/// volume (gain) - relative volume at which the sample is played; 1.0 is normal.
		/// pan - 0.0 is centred, -1.0 is left, 1.0 is right, or ALLEGRO_AUDIO_PAN_NONE.
		/// speed - relative speed at which the sample is played; 1.0 is normal.
		m_pan = table.get<float>("pan");
		m_speed = table.get<float>("speed");
		m_volume = table.get<float>("volume");
	}

	Sound::~Sound()
	{
		al_destroy_sample(m_sound);
	}

	void Sound::play()
	{
		al_play_sample(m_sound, m_volume, m_pan, m_speed, ALLEGRO_PLAYMODE_ONCE, nullptr);
	}

	void Sound::setPan(const float pan)
	{
		m_pan = pan;
	}

	void Sound::setSpeed(const float speed)
	{
		m_speed = speed;
	}

	void Sound::setVolume(const float volume)
	{
		m_volume = volume;
	}
}