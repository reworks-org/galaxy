//
//  ParallaxComponent.hpp
//  REngine3
//
//  Created by reworks on 29/08/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef RENGINE3_PARALLAXCOMPONENT_HPP_
#define RENGINE3_PARALLAXCOMPONENT_HPP_

#include "re/types/Component.hpp"

namespace re
{
	class ParallaxComponent : public Component
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Default Constructor.
		*/
		ParallaxComponent();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Destructor. Cleans up component.
		*/
		~ParallaxComponent() override;

		/*
		* IMPORTS: sol::table from lua script containing component data.
		* EXPORTS: none
		* PURPOSE: Set up the component.
		*/
		void init(sol::table& table) override;

		/*
		* IMPORTS: lua table and entity name
		* EXPORTS: Whether or not to save the changed table data.
		* PURPOSE: debug component, change data, etc.
		*/
		void debugFunction(sol::table& table, const std::string& curEntityName) override;

		
	private:
		std::vector<sf::Sprite>
	};
}

#endif
