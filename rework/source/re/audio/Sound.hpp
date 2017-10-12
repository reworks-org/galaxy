//
//  Sound.hpp
//  rework
//
//  Created by reworks on 12/10/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_SOUND_HPP_
#define REWORK_SOUND_HPP_

#include <allegro5/allegro_audio.h>

#include "sol2/sol.hpp"

namespace re
{
	class Sound
	{
		friend class AudioManager;
	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		Sound(sol::table& table);
		
		///
		/// Destructor.
		///
		~Sound();

	private:
		float m_pan;
		float m_speed;
		float m_volume;

		ALLEGRO_SAMPLE* m_sample;
	};
}

#endif