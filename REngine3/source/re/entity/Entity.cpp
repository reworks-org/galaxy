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
	Entity::Entity(const std::string & id, const std::string & script)
	{
		m_id = id;

		// Create lua state and load it from a script in the VFS.
		sol::state lua;
		lua.script(Locator::Get<VFS>()->ToString(script));

		// Get a table with the components.
		sol::table entityTable = lua["entity"];
		
		if (entityTable["GraphicsComponent"].valid())
		{
			// Create<GraphicsComponent>(new GraphicsComponent(entityTable["GraphicsComponent"]));
		}


	}

	Entity::~Entity()
	{
		m_components.clear();
	}

	std::string Entity::ID() const
	{
		return m_id;
	}

	bool Entity::IsDead() const
	{
		return m_isDead;
	}
}