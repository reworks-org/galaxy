//
//  Sound.cpp
//  rework
//
//  Created by reworks on 12/10/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <boost/filesystem.hpp>

#include "re/services/VFS.hpp"
#include "re/services/ServiceLocator.hpp"

#include "Sound.hpp"

namespace re
{
	Sound::Sound(sol::table& table)
	{
		std::string fn = table.get<std::string>("file");
		std::string ext = boost::filesystem::extension(fn);
		m_sound = al_load_sample_f(Locator::get<VFS>()->open(fn, "r"), ext.c_str());

		m_pan = table.get<float>("pan");
		m_speed = table.get<float>("speed");
		m_volume = table.get<float>("volume");
	}

	Sound::~Sound()
	{
		al_destroy_sample(m_sound);
	}
}