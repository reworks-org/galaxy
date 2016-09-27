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
		* IMPORTS: script - The name of the script in the VFS that defines the entity.
		* EXPORTS: none
		* PURPOSE: Constructs the entity using data from the lua script provided.
		*/
		void Add(const std::string& name, Entity* e);
		
		/*
		* IMPORTS: script - The name of the script in the VFS that defines the entity.
		* EXPORTS: none
		* PURPOSE: Constructs the entity using data from the lua script provided.
		*/
		Entity* Get(const std::string& name);

	private:
		std::unordered_map<std::string, Entity*> m_entitys;
	};
}

#endif