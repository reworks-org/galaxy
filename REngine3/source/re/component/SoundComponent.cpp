//
//  SoundComponent.cpp
//  REngine3
//
//  Created by reworks on 19/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/deps/sol2/sol.hpp"
#include "re/services/vfs/VFS.hpp"
#include "re/services/ServiceLocator.hpp"

#include "SoundComponent.hpp"

namespace re
{
	SoundComponent::SoundComponent()
	{
	}

	SoundComponent::~SoundComponent()
	{
	}

	void SoundComponent::AddSound(const std::string& script)
	{
		sol::state lua;
		lua.script(Locator::Get<VFS>()->ToString(script));

		sol::table sound = lua.get<sol::table>("sound");

		m_sounds.emplace(sound.get<std::string>("id"), std::make_shared<Sound>(sound.get<std::string>("sound")));
	}

	std::shared_ptr<Sound> SoundComponent::Get(const std::string& id)
	{	
		return m_sounds[id];
	}
}