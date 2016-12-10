//
//  Sound.cpp
//  REngine3
//
//  Created by reworks on 19/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/services/vfs/VFS.hpp"
#include "re/services/ServiceLocator.hpp"

#include "Sound.hpp"

namespace re
{
	Sound::Sound(sol::table& table)
	:sf::Sound()
	{
		LoadSound(table.get<std::string>("file"));
		setVolume(table.get<float>("volume"));
		setLoop(table.get<bool>("looping"));
	}

	Sound::~Sound()
	{
	}

	void Sound::LoadSound(const std::string& name)
	{
		m_soundStream.open(name);
		m_soundBuffer.loadFromStream(m_soundStream);
		setBuffer(m_soundBuffer);
	}
}