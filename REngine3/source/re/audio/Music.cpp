//
//  Music.cpp
//  REngine3
//
//  Created by reworks on 19/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/services/vfs/VFS.hpp"
#include "re/scripting/sol2/sol.hpp"
#include "re/services/ServiceLocator.hpp"

#include "Music.hpp"

namespace re
{
	Music::Music(const std::string& script)
	:sf::Music()
	{
		sol::state lua;
		lua.script(Locator::Get<VFS>()->ToString(script));
		sol::table music = lua.get<sol::table>("music");
		
		LoadMusic(music.get<std::string>("file"));
		setVolume(music.get<float>("volume"));
		setLoop(music.get<bool>("looping"));
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