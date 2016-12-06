//
//  B2DCallbacks.cpp
//  REngine3
//
//  Created by reworks on 27/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <utility>

#include <re/physics/Box2DManager.hpp>
#include <re/services/ServiceLocator.hpp>

#include "B2DCallbacks.hpp"

SandboxContact::~SandboxContact()
{
}

void SandboxContact::BeginContact(b2Contact* contact)
{
	re::Entity* a = static_cast<re::Entity*>(contact->GetFixtureA()->GetBody()->GetUserData());
	re::Entity* b = static_cast<re::Entity*>(contact->GetFixtureB()->GetBody()->GetUserData());

	if (a != nullptr && b != nullptr)
	{
		auto map = re::Locator::Get<re::Box2DManager>()->m_collisionFunctions;
		auto tree = map.find(std::make_pair(a->m_name, b->m_name));

		if (tree != map.end())
		{
			tree->second(a, b);
		}
	}
}

void SandboxContact::EndContact(b2Contact* contact)
{
	// We don't really care about this for now.
}