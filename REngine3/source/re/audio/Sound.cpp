//
//  Sound.cpp
//  REngine3
//
//  Created by reworks on 19/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "Sound.hpp"

namespace re
{
	Sound::Sound(const std::string& name)
	:sf::Sound()
	{
		LoadSound(name);
	}

	Sound::~Sound()
	{
	}

	void Sound::LoadSound(const std::string &name)
	{
		m_soundStream.open(name);
		m_soundBuffer.loadFromStream(m_soundStream);
		setBuffer(m_soundBuffer);
	}
}