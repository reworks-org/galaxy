//
//  CollisionComponent.hpp
//  REngine3
//
//  Created by reworks on 10/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_COLLISIONCOMPONENT_HPP_
#define RENGINE3_COLLISIONCOMPONENT_HPP_

#include <SFML/Graphics/Rect.hpp>

#include "re/types/Component.hpp"
#include "re/physics/Box2DManager.hpp"

namespace re
{
	class CollisionComponent : public Component
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Default Constructor.
		*/
		CollisionComponent();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up component.
		*/
		~CollisionComponent() override;

		/*
		* IMPORTS: sol::table containing data.
		* EXPORTS: none
		* PURPOSE: Set up component.
		*/
		void Init(sol::table& table) override;

	public:
		b2Body* m_body;
	};
}

#endif