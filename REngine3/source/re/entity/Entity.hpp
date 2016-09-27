//
//  Entity.hpp
//  REngine3
//
//  Created by reworks on 18/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_ENTITY_HPP_
#define RENGINE3_ENTITY_HPP_

#include <memory>
#include <typeindex>

#include "re/types/Component.hpp"

namespace re
{
	class World;

	class Entity
	{
	public:
		/*
		* IMPORTS: script - The name of the script in the VFS that defines the entity.
		* EXPORTS: none
		* PURPOSE: Constructs the entity using data from the lua script provided.
		*/
		Entity(const std::string& script, sf::Uint64 id, World* world);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Cleanup the object when the program closes.
		*/
		~Entity();

		/*
		* IMPORTS: none
		* EXPORTS: shared_ptr to component of type defined by template.
		* PURPOSE: To retrieve a component of an entity.
		*/
		template<typename T>
		std::shared_ptr<T> Get();

		/*
		* IMPORTS: none
		* EXPORTS: boolean - true if it does have a component.
		* PURPOSE: To find out if an entity has a specific component. Type defined by template.
		*/
		template<typename T>
		bool Has();

	public:
		sf::Uint64 m_id;
		World* m_world;
		ComponentList* m_components;
		std::vector<std::type_index> m_systemIds;

		bool m_isDead = false;
	};

	template<typename T>
	std::shared_ptr<T> Entity::Get()
	{
		return std::dynamic_pointer_cast<T>(m_components.find(std::type_index(typeid(T)))->second);
	}

	template<typename T>
	bool Entity::Has()
	{
		if (m_components.find(std::type_index(typeid(T))) != m_components.end())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

#endif