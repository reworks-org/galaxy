//
//  PhysicsComponent.hpp
//  REngine3
//
//  Created by reworks on 10/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_PHYSICSCOMPONENT_HPP_
#define RENGINE3_PHYSICSCOMPONENT_HPP_

#include "re/types/Component.hpp"
#include "re/physics/Box2D/Dynamics/b2Body.h"

namespace re
{
	class PhysicsComponent : public Component
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Default Constructor.
		*/
		PhysicsComponent();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up component.
		*/
		~PhysicsComponent() override;

		/*
		* IMPORTS: sol::table containing data.
		* EXPORTS: none
		* PURPOSE: Set up component.
		*/
		void init(sol::table& table) override;

	public:
		b2Body* m_body;
		bool m_isMovingVertically = false;
		bool m_isMovingHoritontally = false;
	};
}

#endif
