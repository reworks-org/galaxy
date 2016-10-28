//
//  Component.hpp
//  REngine3
//
//  Created by reworks on 18/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_COMPONENT_HPP_
#define RENGINE3_COMPONENT_HPP_

#include <SFML/System/Time.hpp>

#include <typeindex>
#include <unordered_map>

#include "re/scripting/sol2/sol.hpp"

namespace re
{
	class Component
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Abstract virtual destructor for components.
		*/
		virtual ~Component() {}

		/*
		* IMPORTS: sol::table from lua script containing component data.
		* EXPORTS: none
		* PURPOSE: Set up the component.
		*/
		virtual void Init(sol::table& table) = 0;

		/*
		* IMPORTS: delta time
		* EXPORTS: none
		* PURPOSE: Update the component.
		*/
		virtual void Update(sf::Time dt) {}
	};

	typedef std::unordered_map<std::type_index, std::shared_ptr<Component>> ComponentList;
}

#endif