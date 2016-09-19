//
//  Music.cpp
//  REngine3
//
//  Created by reworks on 19/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "Music.hpp"

namespace re
{
	Music::Music(const std::string& name)
	:sf::Music()
	{
		LoadMusic(name);
	}

	Music::~Music()
	{
	}

	void Music::LoadMusic(const std::string & name)
	{
		m_musicStream.open(name);
		openFromStream(m_musicStream);
	}
}