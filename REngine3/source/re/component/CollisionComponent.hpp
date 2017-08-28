//
//  CollisionComponent.cpp
//  REngine3
//
//  Created by reworks on 28/08/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef RENGINE3_COLLISIONCOMPONENT_HPP_
#define RENGINE3_COLLISIONCOMPONENT_HPP_

#include <SFML/Graphics/Rect.hpp>

#include "re/types/Component.hpp"

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
		void init(sol::table& table) override;

		/*
		* IMPORTS: lua table and entity name
		* EXPORTS: Whether or not to save the changed table data.
		* PURPOSE: debug component, change data, etc.
		*/
		void debugFunction(sol::table& table, const std::string& curEntityName) override;

	public:
		sf::Rect<int> m_rect;
	};
}

#endif
