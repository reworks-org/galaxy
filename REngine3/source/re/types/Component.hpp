//
//  Component.hpp
//  REngine3
//
//  Created by reworks on 18/07/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef RENGINE3_COMPONENT_HPP_
#define RENGINE3_COMPONENT_HPP_

#include <typeindex>
#include <unordered_map>

#include "sol2/sol.hpp"

namespace re
{
	class Component
	{
        friend class Entity;
	public:
		///
		/// Virtual destructor to be inherited by child classes.
		///
		virtual inline ~Component() {};

		///
		/// Initializes the component data.
		///
		/// \param sol::table Table containing component data from a sol::state.
		///
		virtual void init(sol::table& table) = 0;

    protected:
		///
		/// \brief Modify a component in-game.
		///
		/// Uses IMGUI to modify a component and its script from inside the game. Allows you to tweak settings
		/// and config until you find the right settings.
		///
		/// \param sol::table A table containing data about component.
		/// \param curEntityName The current name of the entity this function is being called from.
		///
        virtual void debug(sol::table& table, const std::string& curEntityName) = 0;
	};

	typedef std::unordered_map<std::type_index, std::unique_ptr<Component>> ComponentList;
	typedef std::unordered_map<std::string, ComponentList> ComponentHolder;
}

#endif
