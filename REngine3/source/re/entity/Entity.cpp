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
#include "re/component/PositionComponent.hpp"
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

		if (entityTable["AnimatedSpriteComponent"].valid())
		{
			Create<AnimatedSpriteComponent>(std::shared_ptr<AnimatedSpriteComponent>(new AnimatedSpriteComponent(entityTable["AnimatedSpriteComponent"])));
		}

		if (entityTable["SpriteComponent"].valid())
		{
			Create<SpriteComponent>(std::shared_ptr<SpriteComponent>(new SpriteComponent(entityTable["SpriteComponent"])));
		}

		if (entityTable["TextComponent"].valid())
		{
			Create<TextComponent>(std::shared_ptr<TextComponent> (new TextComponent(entityTable["TextComponent"])));
		}

		if (entityTable["PositionComponent"].valid())
		{
			Create<PositionComponent>(std::shared_ptr<PositionComponent>(new PositionComponent(entityTable["PositionComponent"])));
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