//
//  MusicComponent.cpp
//  REngine3
//
//  Created by reworks on 19/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/deps/sol2/sol.hpp"
#include "re/services/vfs/VFS.hpp"
#include "re/services/ServiceLocator.hpp"

#include "MusicComponent.hpp"

namespace re
{
	MusicComponent::MusicComponent()
	{
	}

	MusicComponent::~MusicComponent()
	{
	}

	void MusicComponent::Init(sol::table& table)
	{
	}

	void MusicComponent::AddMusic(const std::string& script)
	{
		sol::state lua;
		lua.script(Locator::Get<VFS>()->ToString(script));

		sol::table music = lua.get<sol::table>("music");

		m_music.emplace(music.get<std::string>("id"), std::make_shared<Music>(music.get<std::string>("music")));
	}

	std::shared_ptr<Music> MusicComponent::Get(const std::string& id)
	{
		return m_music[id];
	}
}