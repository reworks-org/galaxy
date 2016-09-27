//
//  System.hpp
//  REngine3
//
//  Created by reworks on 10/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_SYSTEM_HPP_
#define RENGINE3_SYSTEM_HPP_

#include "re/types/Types.hpp"
#include "re/entity/Entity.hpp"

namespace re
{
	class System
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Virtual destructor for systems.
		*/
		virtual ~System();

		/*
		* IMPORTS: id of entity to remove
		* EXPORTS: none
		* PURPOSE: Remove an entitys components from the system.
		*/
		void RemoveEntity(sf::Uint64 e);

		/*
		* IMPORTS: id of entity to add and its component list.
		* EXPORTS: none
		* PURPOSE: Add an entitys components from the system.
		*/
		void AddEntity(Entity* e);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Get stored components.
		*/
		std::unordered_map<sf::Uint64, Entity*>& GetEntitys();

	protected:
		std::unordered_map<sf::Uint64, Entity*> m_entitys;
	};
}

#endif