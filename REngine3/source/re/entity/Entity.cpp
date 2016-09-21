//
//  Entity.cpp
//  REngine3
//
//  Created by reworks on 18/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/deps/sol2/sol.hpp"
#include "re/services/vfs/VFS.hpp"
#include "re/services/ServiceLocator.hpp"

#include "re/component/TextComponent.hpp"
#include "re/component/SoundComponent.hpp"
#include "re/component/MusicComponent.hpp"
#include "re/component/SpriteComponent.hpp"
#include "re/component/AnimatedSpriteComponent.hpp"

#include "Entity.hpp"

#include <iostream>

namespace re
{
	Entity::Entity(const std::string & script)
	{
		// Create lua state and load it from a script in the VFS.
		sol::state lua;
		lua.script(Locator::Retrieve<VFS>()->ToString(script));

		// Get a table with the components.
		sol::table entityTable = lua.get<sol::table>("entity");

		// Get key-value pairs from table
		std::map<std::string, sol::table> m_keyValuePair;
		entityTable.for_each([&](std::pair<sol::object, sol::object> pair) {
			m_keyValuePair.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
		});

		for (auto& it : m_keyValuePair)
		{
			if (it.first == "AnimatedSpriteComponent")
			{
				Create<AnimatedSpriteComponent>(std::make_shared<AnimatedSpriteComponent>(it.second));
			}
			else if (it.first == "SpriteComponent")
			{
				Create<SpriteComponent>(std::make_shared<SpriteComponent>(it.second));
			}
			else if (it.first == "TextComponent")
			{
				Create<TextComponent>(std::make_shared<TextComponent>(it.second));
			}
			else if (it.first == "MusicComponent")
			{
				Create<MusicComponent>(std::make_shared<MusicComponent>());
			}
			else if (it.first == "SoundComponent")
			{
				Create<SoundComponent>(std::make_shared<SoundComponent>());
			}
		}
	}

	Entity::~Entity()
	{
		m_components.clear();
	}

	void Entity::SetID(const std::string & id)
	{
		m_id = id;
	}

	std::string Entity::ID() const
	{
		return m_id;
	}
}