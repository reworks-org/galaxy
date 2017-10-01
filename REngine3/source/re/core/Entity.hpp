//
//  Entity.hpp
//  REngine3
//
//  Created by reworks on 18/07/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef RENGINE3_ENTITY_HPP_
#define RENGINE3_ENTITY_HPP_

#include <map>
#include <memory>
#include <typeindex>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include "re/utility/Log.hpp"
#include "re/types/Component.hpp"

namespace re
{
	class Entity
	{
		friend class boost::serialization::access;
		friend class World;
	public:
		///
		/// Construct an entity from the data provided.
		///
		/// \param cl The component holder from the World.
		/// \param script The script containing
		///
		Entity(const std::string& script, ComponentHolder& cl);

		///
		/// Disabled default constructor.
		///
		Entity() = delete;
		
		///
		/// Disabled copy constructor.
		///
		Entity(const Entity& e) = delete;

		///
		/// Cleanup entity.
		///
		~Entity();

		///
		/// Checks if entity is dead.
		///
		/// \returns True if dead.
		///
		bool isDead() const;

		///
		/// Get a component.
		///
		/// \param T Type of component to get.
		///
		/// \return Returns pointer to component, or nullptr if not success.
		///
		template<typename T>
		T* get();
        
		///
		/// Checks to see if entity has a component.
		///
		/// \param T Type of component to check.
		///
		/// \return Boolean. True if entity has component.
		///
		template<typename T>
		bool has();

	private:
		///
		/// Boost.Serialization function
		///
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & m_isDead;
		}

		///
		/// Debug function for Debug Manager.
		///
		void useComponentDebugFunction(const std::string& componentName, sol::table& table, const std::string& curEntityName);

	public:
		std::string m_name;
		ComponentList* m_components;
		std::map<std::string, std::type_index> m_systemIds;

	private:
		bool m_isDead;
	};

	template<typename T>
	T* Entity::get()
	{
		auto it = m_components->find(std::type_index(typeid(T)));
		if (it != m_components->end())
		{
			return dynamic_cast<T*>(it->second.get());
		}
		else
		{
			RE_LOG(LogLevel::WARNING, "Tried to access non-existant component", "Entity::get", "Entity.cpp", 118);
			return nullptr;
		}
	}

	template<typename T>
	bool Entity::has()
	{
		bool out;

		out = m_components->find(std::type_index(typeid(T))) != m_components->end() ? true : false;

		return out;
	}
}

#endif