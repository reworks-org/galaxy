//
//  Sound.cpp
//  rework
//
//  Created by reworks on 12/10/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "re/services/VFS.hpp"
#include "re/services/ServiceLocator.hpp"

#include "Sound.hpp"

namespace re
{
	Sound::Sound(sol::table& table)
	{
		std::string fn = table.get<std::string>("file");
		m_sound = al_load_sample(fn.c_str());

		// volume (gain) - relative volume at which the sample is played; 1.0 is normal.
		// pan - 0.0 is centred, -1.0 is left, 1.0 is right, or ALLEGRO_AUDIO_PAN_NONE.
		// speed - relative speed at which the sample is played; 1.0 is normal.
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
		al_play_sample(m_sound, m_volume, m_pan, m_speed, ALLEGRO_PLAYMODE_ONCE, NULL);
	}

	void Sound::setPan(float pan)
	{
		m_pan = pan;
	}

	void Sound::setSpeed(float speed)
	{
		m_speed = speed;
	}

	void Sound::setVolume(float volume)
	{
		m_volume = volume;
	}
}