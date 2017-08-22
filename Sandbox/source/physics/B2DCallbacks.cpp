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
#include <re/component/PhysicsComponent.hpp>

#include "B2DCallbacks.hpp"

SandboxContact::~SandboxContact()
{
}

void SandboxContact::BeginContact(b2Contact* contact)
{
	std::string* aa = static_cast<std::string*>(contact->GetFixtureA()->GetUserData());
	std::string* bb = static_cast<std::string*>(contact->GetFixtureB()->GetUserData());

	std::string a = *(aa);
	std::string b = *(bb);

	if (a != "" && b != "")
	{
		auto map = re::Locator::get<re::Box2DManager>()->m_collisionFunctions;
		auto tree = map.find(std::make_pair(a, b));

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