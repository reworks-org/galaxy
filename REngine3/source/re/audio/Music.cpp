//
//  Music.cpp
//  REngine3
//
//  Created by reworks on 19/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/services/vfs/VFS.hpp"
#include "re/services/ServiceLocator.hpp"

#include "Music.hpp"

namespace re
{
	Music::Music(sol::table& table)
	:sf::Music()
	{
		LoadMusic(table.get<std::string>("file"));
		setVolume(table.get<float>("volume"));
		setLoop(table.get<bool>("looping"));
	}

	Music::~Music()
	{
	}

	void Music::LoadMusic(const std::string& name)
	{
		m_musicStream.open(name);
		openFromStream(m_musicStream);
	}
}