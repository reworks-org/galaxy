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
#include "solar/core/Storage.hpp"
#include "solar/core/SparseSet.hpp"

namespace sr
{
	using ComponentVector = std::vector<std::unique_ptr<BaseStorage>>;
	using SystemVector = std::vector<std::unique_ptr<System>>;

	class Heliosphere
	{
	public:
		Heliosphere();
		~Heliosphere();

		Entity create() noexcept;

		template<typename Component, typename... Args>
		void add(Entity entity, Args&&... args) noexcept;

		template<typename Component>
		Component* get(Entity entity) noexcept;

		template<typename... Components>
		decltype(auto) get(Entity entity) noexcept;

		template<typename System, typename... Args>
		void add(Args&&... args) noexcept;

		template<typename System>
		System* get() noexcept;

		///
		/// Destroys an entity and all assosiated components.
		///
		void destroy(Entity entity);

	private:
		template<typename Component>
		Component* extract(Entity entity, SR_INTEGER type) noexcept;

	private:
		SR_INTEGER m_nextID;
		SparseSet<Entity> m_entities;
		ComponentVector m_components;
		SystemVector m_systems;
	};

	template<typename Component, typename... Args>
	inline void Heliosphere::add(Entity entity, Args&& ...args) noexcept
	{
		auto type = cuid::uid<Component>();

		if (type >= m_components.size())
		{
			m_components.resize(type + 1);
		}

		// Ensure leftover references to unique pointer are destroyed.
		{
			// If null ptr, then no storage for this component exists.
			if (!m_components[type])
			{
				// Use polymorphism to ensure type erasure.
				m_components[type] = std::make_unique<ComponentStorage<Component>>();
			}

			// Now convert the storage to the type we want to access.
			ComponentStorage<Component>* derived = static_cast<ComponentStorage<Component>*>(m_components[type].get());

			// Access to components of that type.
			// Make sure to access integer.
			if (entity >= derived->m_components.size())
			{
				derived->m_components.resize(entity + 1);
			}

			// Component is then placed at entitys id. i.e. [entity] = component.
			derived->m_components[entity] = Component(std::forward<Args>(args)...);
		}
	}

	template<typename Component>
	inline Component* Heliosphere::get(Entity entity) noexcept
	{
		auto type = cuid::uid<Component>();
		static_assert(type > m_components.size(), "Component type does not exist!");

		return extract<Component>(entity, type);
	}

	template<typename... Components>
	inline decltype(auto) Heliosphere::get(Entity entity) noexcept
	{
		std::vector<SR_INTEGER> type_id_list;
		
		//(type_id_list.push_back(cuid::uid<Components>()), ...);
		//(extract<Components>(entity, cuid::uid<Components>()), ...);

		auto tuple = std::make_tuple(extract<Components>(entity, cuid::uid<Components>())...);

		//auto tuple = std::tuple_cat(extract<Components>(entity, cuid::uid<Components>())...);

		
		// FOR DEBUG PURPOSES:
		/*
		for (auto i = 0; i < type_id_list.size(); i++)
		{
			std::cout << "id: " << type_id_list[i] << std::endl;
		}
		*/
		
		return tuple;
	}

	template<typename System, typename ...Args>
	inline void Heliosphere::add(Args&& ...args) noexcept
	{
		auto type = suid::uid<System>();
		
		if (type > m_systems.size())
		{
			m_systems.resize(type + 1, nullptr);
		}

		m_systems[type] = std::make_unique<System>(std::forward<Args>(args)...);
	}

	template<typename System>
	inline System* Heliosphere::get() noexcept
	{
		auto type = suid::uid<System>();

		if (type > m_systems.size())
		{
			return nullptr;
		}
		else
		{
			return m_systems[type].get();
		}
	}

	template<typename Component>
	inline Component* Heliosphere::extract(Entity entity, SR_INTEGER type) noexcept
	{
		ComponentStorage<Component>* derived = static_cast<ComponentStorage<Component>*>(m_components[type].get());
		return &(derived->m_components[entity]);
	}
}

#endif