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

		if (entityTable["AnimationComponent"].valid())
		{
			// Create<AnimationComponent>(new AnimationComponent(entityTable["AnimationComponent"]));
		}

		if (entityTable["GraphicsComponent"].valid())
		{
			// Create<GraphicsComponent>(new GraphicsComponent(entityTable["GraphicsComponent"]));
		}

		if (entityTable["TextComponent"].valid())
		{
			// Create<TextComponent>(new TextComponent(entityTable["TextComponent"]));
		}

		if (entityTable["PositionComponent"].valid())
		{
			// Create<PositionComponent>(new PositionComponent(entityTable["PositionComponent"]));
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