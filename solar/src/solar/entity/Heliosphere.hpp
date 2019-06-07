///
/// Heliosphere.hpp
///
/// solar
/// See LICENSE.txt.
///

/*
 Entitys ID used as insertion for std::vector so [entity] = components (but how to get which one?).
 components stored in groupings of type, assigned unique id, then paired with their entity.
 stored in std::vector
 

 so:
 std::vector<PackedComponents> with element being = entity.
 packed components = std::vector?



 So the element to be inserted is the type of component, then an array of those components of that type.
 the key is the type of component, the value is components of the same type, which is in a data structure, paired with an entity.


*/

#ifndef SOLAR_HELIOSPHERE_HPP_
#define SOLAR_HELIOSPHERE_HPP_

#include <tuple>
#include <memory>
#include <iostream>

#include "solar/core/UID.hpp"
#include "solar/core/System.hpp"
#include "solar/core/ExtendedSet.hpp"

namespace sr
{
	using SystemContainer = std::vector<std::unique_ptr<System>>;
	using ComponentContainer = std::vector<std::unique_ptr<SparseSet<Entity>>>;

	class Heliosphere
	{
	public:
		Heliosphere();
		~Heliosphere();

		Entity create() noexcept;

		template<typename Component, typename... Args>
		void add(Entity entity, Args&&... args) noexcept;

		template<typename Component>
		Component* get(Entity entity);

		template<typename... Components>
		decltype(auto) get(Entity entity) noexcept;

		template<typename System, typename... Args>
		void add(Args&&... args);

		template<typename System>
		System* get();

		///
		/// Destroys an entity and all assosiated components.
		///
		void destroy(Entity entity);

		///
		/// Pass on event to all systems.
		///
		void event(const Event& event);

		///
		/// Update all systems.
		///
		void update(const DeltaTime time);

	private:
		template<typename Component>
		Component* extract(Entity entity);

	private:
		///
		/// Counter for free entity ids.
		///
		SR_INTEGER m_nextID;

		///
		/// Stores entitys.
		///
		SparseSet<Entity> m_entities;

		///
		/// Stores polymorphic ExtendedSets.
		///
		ComponentContainer m_data;

		SystemContainer m_systems;
	};

	template<typename Component, typename... Args>
	inline void Heliosphere::add(Entity entity, Args&& ...args) noexcept
	{
		auto type = cuid::uid<Component>();

		if (type >= m_data.size())
		{
			m_data.resize(type + 1);
		}

		// Ensure leftover references to unique pointer are destroyed.
		{
			// If null ptr, then no storage for this component exists.
			if (!m_data[type])
			{
				// Use polymorphism to ensure type erasure.
				m_data[type] = std::make_unique<ExtendedSet<Component>>();
			}

			// Now convert the storage to the type we want to access.
			ExtendedSet<Component>* derived = static_cast<ExtendedSet<Component>*>(m_data[type].get());

			derived->add(entity, std::forward<Args>(args)...);
		}
	}

	template<typename Component>
	inline Component* Heliosphere::get(Entity entity)
	{
		auto type = cuid::uid<Component>();

		if (type > m_data.size())
		{
			throw std::out_of_range("Attempted to access a component type that doesnt exist!");
		}

		return extract<Component>(entity, type);
	}

	template<typename... Components>
	inline decltype(auto) Heliosphere::get(Entity entity) noexcept
	{
		return std::make_tuple(extract<Components>(entity)...);

		//std::vector<SR_INTEGER> type_id_list;
		
		//(type_id_list.push_back(cuid::uid<Components>()), ...);
		//(extract<Components>(entity, cuid::uid<Components>()), ...);

		//auto tuple = std::make_tuple(extract<Components>(entity, cuid::uid<Components>())...);

		//auto tuple = std::tuple_cat(extract<Components>(entity, cuid::uid<Components>())...);

		
		// FOR DEBUG PURPOSES:
		/*
		for (auto i = 0; i < type_id_list.size(); i++)
		{
			std::cout << "id: " << type_id_list[i] << std::endl;
		}
		*/
	}

	template<typename System, typename ...Args>
	inline void Heliosphere::add(Args&&... args)
	{
		auto type = suid::uid<System>();
		if (type > m_systems.size())
		{
			m_systems.resize(type + 1);
		}

		m_systems[type] = std::make_unique<System>(std::forward<Args>(args)...);
	}

	template<typename System>
	inline System* Heliosphere::get()
	{
		auto type = suid::uid<System>();
		if (type > m_systems.size())
		{
			throw std::out_of_range("Attempted to access a system type that doesnt exist!");
		}

		return m_systems[type].get();
	}

	template<typename Component>
	inline Component* Heliosphere::extract(Entity entity)
	{
		ExtendedSet<Component>* derived = static_cast<ExtendedSet<Component>*>(m_data[cuid::uid<Component>()].get());
		return derived->get(entity);
	}
}

#endif