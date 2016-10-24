//
//  EntityManager.hpp
//  REngine3
//
//  Created by reworks on 27/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_ENTITYMANAGER_HPP_
#define RENGINE3_ENTITYMANAGER_HPP_

#include "re/types/Service.hpp"
#include "re/entity/Entity.hpp"

namespace re
{
	class EntityManager : public Service
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Cleanup entitys.
		*/
		~EntityManager() override;

		/*
		* IMPORTS: name and pointer to entity.
		* EXPORTS: none
		* PURPOSE: Add an entity to the map.
		*/
		void Add(const std::string& name, std::shared_ptr<Entity> e);

		/*
		* IMPORTS: name - name of the entity in the manager.
		* EXPORTS: pointer to the entity.
		* PURPOSE: Retrieve an entity from the manager.
		*/
		std::shared_ptr<Entity> Get(const std::string& name);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean the manager.
		*/
		void Clean();

		/*
		* IMPORTS: none
		* EXPORTS: reference to map of the entitys.
		* PURPOSE: Get the map of the entitys.
		*/
		std::unordered_map<std::string, std::shared_ptr<Entity>>& GetMap();

	private:
		std::unordered_map<std::string, std::shared_ptr<Entity>> m_entitys;
	};
}

#endif