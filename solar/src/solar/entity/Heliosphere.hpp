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

#include <memory>

#include "solar/core/UID.hpp"
#include "solar/core/Storage.hpp"
#include "solar/core/SparseSet.hpp"

namespace sr
{
	class Heliosphere
	{
	public:
		Entity create() noexcept;

		template<typename Component, typename... Args>
		Component add(Entity entity, Args&&... args) noexcept;

		template<typename Component>
		Component* get(Entity entity) noexcept;

		template<typename... Components>
		std::tuple<Components*...> get(Entity entity) noexcept;

	private:
		SparseSet<Entity> m_entities;
		std::vector<std::unique_ptr<BaseStorage>> m_components;
	};

	template<typename Component, typename... Args>
	inline Component Heliosphere::add(Entity entity, Args&& ...args) noexcept
	{
		auto type = cuid::uid<Component>();

		// Avoid 0 senario, because ID counter starts at 1.
		if (type > m_components.size())
		{
			m_components.reserve(type + 1);
		}

		// Ensure leftover references to unique pointer are destroyed.
		{
			// Get internal pointer.
			BaseStorage* ptr = m_components[type].get();

			// If null ptr, then no storage for this component exists.
			if (!ptr)
			{
				// Use polymorphism to ensure type erasure.
				m_components[type] = std::make_unique<ComponentStorage<Component>>();
			}

			// Now convert the storage to the type we want to access.
			ComponentStorage<Component>* derived = static_cast<ComponentStorage<Component>*>(ptr);

			// Access to components of that type.
			// Make sure to access integer.
			if (entity > derived->m_components.size())
			{
				derived->m_components.reserve(entity + 1);
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

		ComponentStorage<Component>* derived = static_cast<ComponentStorage<Component>*>(m_components[type]);
		return &(derived->m_components[entity]);
	}

	template<typename... Components>
	inline std::tuple<Components*...> Heliosphere::get(Entity entity) noexcept
	{
		// TODO
	}
}

#endif