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
#include "re/component/SpriteComponent.hpp"
#include "re/component/AnimatedSpriteComponent.hpp"

#include "Entity.hpp"

namespace re
{
	Entity::Entity(const std::string & script)
	{
		// Create lua state and load it from a script in the VFS.
		sol::state lua;
		lua.script(Locator::Get<VFS>()->ToString(script));

		// Get a table with the components.
		sol::table entityTable = lua["entity"];
	
		// Construct components from lua script.
		sol::table ac = entityTable["AnimatedSpriteComponent"];
		sol::table sc = entityTable["SpriteComponent"];
		sol::table tc = entityTable["TextComponent"];

		if (ac.valid())
		{
			Create<AnimatedSpriteComponent>(std::make_shared<AnimatedSpriteComponent>(ac));
		}

 		if (sc.valid())
		{
			Create<SpriteComponent>(std::make_shared<SpriteComponent>(sc));
		}
		
		if (tc.valid())
		{
			Create<TextComponent>(std::make_shared<TextComponent>(tc));
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