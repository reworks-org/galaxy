//
//  Music.hpp
//  rework
//
//  Created by reworks on 13/10/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_MUSIC_HPP_
#define REWORK_MUSIC_HPP_

#include <allegro5/allegro_audio.h>

#include "sol2/sol.hpp"

namespace re
{
	class Music
	{
	public:
		Music(sol::table& table);
		~Music();

	private:
		ALLEGRO_SAMPLE* m_music;
		ALLEGRO_SAMPLE_INSTANCE* m_instance;
	};
}

#endif