//
//  B2DCallbacks.hpp
//  REngine3
//
//  Created by reworks on 27/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef SANDBOX_B2DCALLBACKS_HPP_
#define SANDBOX_B2DCALLBACKS_HPP_

#include <string>

#include <re/entity/Entity.hpp>
#include <re/physics/Box2D/Box2d.h>

class SandboxContact : public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact) override
	{
		auto userData = contact->GetFixtureA()->GetBody()->GetUserData();

		if (userData)
		{
			// http://pastebin.com/HK6MUVfd
		}
	}
};

#endif