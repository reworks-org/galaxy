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
#include <unordered_map>

#include "re/component/Component.hpp"

namespace re
{
	class Entity
	{
	public:
		/*
		* IMPORTS: script - The name of the script in the VFS that defines the entity.
		* EXPORTS: none
		* PURPOSE: Constructs the entity using data from the lua script provided.
		*/
		Entity(const std::string& script);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Cleanup the object when the program closes.
		*/
		~Entity();

		/*
		* IMPORTS: component - Takes a component of the type defined by the template. Use 'new' to create component of the correct type.
		* EXPORTS: none
		* PURPOSE: Create a specific component and cast down to store. Can only have one of each type.
		*/
		template<typename T>
		void Create(std::unique_ptr<Component> component);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Deletes a component of a specific type. Use template to define which component.
		*/
		template<typename T>
		void Delete();

		/*
		* IMPORTS: none
		* EXPORTS: unique_ptr to component of type defined by template.
		* PURPOSE: To retrieve a component of an entity.
		*/
		template<typename T>
		std::unique_ptr<T> Get();

		/*
		* IMPORTS: none
		* EXPORTS: boolean - true if it does have a component.
		* PURPOSE: To find out if an entity has a specific component. Type defined by template.
		*/
		template<typename T>
		bool Has();

		/*
		* IMPORTS: std::string - the id.
		* EXPORTS: none
		* PURPOSE: Set the ID of the entity. Do NOT call this. This is handled for you!
		*/
		void SetID(const std::string& id);

		/*
		* IMPORTS: none
		* EXPORTS: ID as a std::string
		* PURPOSE: To retrieve the ID of the entity.
		*/
		std::string ID() const;

	private:
		std::string m_id;
		std::unordered_map<std::type_index, std::unique_ptr<Component>> m_components;

	public:
		bool m_isDead = false;
	};

	template<typename T>
	void Entity::Create(std::unique_ptr<Component> component)
	{
		if (m_Components.find(std::type_index(typeid(T))) != m_Components.end())
		{
			RE_LOG(LogLevel::WARNING, "Tried to create a pre-existing component");
		}
		else
		{
			m_Components[typeid(T)] = std::move(component);
		}
	}

	template<typename T>
	void Entity::Delete()
	{
		if (m_Components.find(std::type_index(typeid(T))) != m_Components.end())
		{
			m_Components.erase(typeid(T));
		}
		else
		{
			RE_LOG(LogLevel::WARNING, "Tried to remove a non-existant component");
		}
	}

	template<typename T>
	std::unique_ptr<T> Entity::Get()
	{
		auto it = m_Components.find(std::type_index(typeid(T)));

		if (it != m_Components.end())
		{
			return std::dynamic_pointer_cast<T>(it->second);
		}
		else
		{
			return nullptr;
		}
	}

	template<typename T>
	bool Entity::Has()
	{
		if (m_Components.find(std::type_index(typeid(T))) != m_Components.end())
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