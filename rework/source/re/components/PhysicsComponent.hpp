//
//  PhysicsComponent.hpp
//  rework
//
//  Created by reworks on 10/11/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_PHYSICSCOMPONENT_HPP_
#define REWORK_PHYSICSCOMPONENT_HPP_

#include "sol2/sol.hpp"
#include "entityx/Entity.h"
#include "Box2D/Dynamics/b2Body.h"

namespace re
{
	class PhysicsComponent
	{
	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		PhysicsComponent(entityx::Entity& e, sol::table& table);

		///
		/// Destructor.
		///
		~PhysicsComponent();

		///
		/// Calls imgui debug functions. Don't call this, done for you by debugmanager.
		///
		void debug();

	public:
		b2Body* m_body;
	};
}

#endif

/*
ar & m_body->GetPosition().x;
ar & m_body->GetPosition().y;
ar & m_body->GetAngle();
ar & m_body->GetAngularVelocity();
ar & m_body->GetLinearVelocity().x;
ar & m_body->GetLinearVelocity().y;

m_body->SetTransform(b2Vec2(x, y), angle);
m_body->SetAngularVelocity(angleVelocity);
m_body->SetLinearVelocity(b2Vec2(linearX, linearY));
*/