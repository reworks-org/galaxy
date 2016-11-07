//
//  Entity.hpp
//  REngine3
//
//  Created by reworks on 18/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_ENTITY_HPP_
#define RENGINE3_ENTITY_HPP_

#include <map>
#include <memory>
#include <typeindex>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include "re/types/Types.hpp"
#include "re/types/Component.hpp"

namespace re
{
	class World;

	class Entity
	{
		friend class boost::serialization::access;
		friend class World;
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Default constructor.
		*/
		Entity();

		/*
		* IMPORTS: script - The name of the script in the VFS that defines the entity. Pointer to world.
		* EXPORTS: none
		* PURPOSE: Constructs the entity using data from the lua script provided.
		*/
		void Init(const std::string& script, ComponentHolder& cl);

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

	private:
		// Boost.Serialization function
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & m_isDead;
		}

	public:
		std::string m_name;
		ComponentList* m_components;
		std::map<std::string, std::type_index> m_systemIds;

	private:
		bool m_isDead = false;
	};

	template<typename T>
	std::shared_ptr<T> Entity::Get()
	{
		return std::dynamic_pointer_cast<T>(m_components->find(std::type_index(typeid(T)))->second);
	}

	template<typename T>
	bool Entity::Has()
	{
		if (m_components->find(std::type_index(typeid(T))) != m_components->end())
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