//
//  Entity.cpp
//  REngine3
//
//  Created by reworks on 18/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <map>

#include "re/deps/sol2/sol.hpp"
#include "re/services/vfs/VFS.hpp"
#include "re/services/ServiceLocator.hpp"

#include "re/component/TextComponent.hpp"
#include "re/component/SoundComponent.hpp"
#include "re/component/MusicComponent.hpp"
#include "re/component/EventComponent.hpp"
#include "re/component/SpriteComponent.hpp"
#include "re/component/PositionComponent.hpp"
#include "re/component/AnimatedSpriteComponent.hpp"
#include "re/systems/RenderSystem.hpp"
#include "re/app/World.hpp"

namespace re
{
	Entity::Entity(const std::string& script, sf::Uint64 id)
	{
		m_id = id;

		// Create lua state and load it from a script in the VFS.
		sol::state lua;
		lua.script(Locator::Get<VFS>()->ToString(script));

		// Get a table with the components.
		sol::table entity = lua.get<sol::table>("entity");

		m_name = entity.get<std::string>("name");

		// Get key-value pairs from table
		std::map<std::string, sol::table> m_keyValuePair;
		entity.for_each([&](std::pair<sol::object, sol::object> pair) {
			m_keyValuePair.insert({ pair.first.as<std::string>(), pair.second.as<sol::table>() });
		});

		for (auto& it : m_keyValuePair)
		{
			if (it.first == "AnimatedSpriteComponent")
			{
				m_components[typeid(AnimatedSpriteComponent)] = std::make_shared<AnimatedSpriteComponent>(it.second);
			}
			else if (it.first == "SpriteComponent")
			{
				m_components[typeid(SpriteComponent)] = std::make_shared<SpriteComponent>(it.second);
			}
			else if (it.first == "TextComponent")
			{
				m_components[typeid(TextComponent)] = std::make_shared<TextComponent>(it.second);
			}
			else if (it.first == "MusicComponent")
			{
				m_components[typeid(MusicComponent)] = std::make_shared<MusicComponent>();
			}
			else if (it.first == "SoundComponent")
			{
				m_components[typeid(SoundComponent)] = std::make_shared<SoundComponent>();
			}
			else if (it.first == "EventComponent")
			{
				m_components[typeid(EventComponent)] = std::make_shared<EventComponent>();
			}
			else if (it.first == "PositionComponent")
			{
				m_components[typeid(PositionComponent)] = std::make_shared<PositionComponent>(it.second);
			}
		}
	}

	Entity::~Entity()
	{
		m_systemIds.clear();
		m_components.clear();
	}
}