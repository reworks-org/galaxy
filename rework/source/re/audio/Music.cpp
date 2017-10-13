//
//  Music.cpp
//  rework
//
//  Created by reworks on 13/10/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "Music.hpp"

namespace re
{
	Music::Music(sol::table& table)
	{
		std::string fn = table.get<std::string>("file");
		std::string ext = boost::filesystem::extension(fn);

		m_m
	}

	Music::~Music()
	{
		al_destroy_sample_instance(m_instance);
		al_destroy_sample(m_music);
	}
}