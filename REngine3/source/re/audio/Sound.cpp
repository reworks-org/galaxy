//
//  Sound.cpp
//  REngine3
//
//  Created by reworks on 19/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/services/vfs/VFS.hpp"
#include "re/scripting/sol2/sol.hpp"
#include "re/services/ServiceLocator.hpp"

#include "Sound.hpp"

namespace re
{
	Sound::Sound(const std::string& script)
	:sf::Sound()
	{
		sol::state lua;
		lua.script(Locator::Get<VFS>()->ToString(script));
		sol::table sound = lua.get<sol::table>("sound");

		LoadSound(sound.get<std::string>("file"));
		setVolume(sound.get<float>("volume"));
		setLoop(sound.get<bool>("looping"));
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