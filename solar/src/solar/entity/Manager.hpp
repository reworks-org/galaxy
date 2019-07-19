///
/// Manager.hpp
///
/// solar
/// See LICENSE.txt.
///

#ifndef SOLAR_MANAGER_HPP_
#define SOLAR_MANAGER_HPP_

#include <tuple>
#include <memory>
#include <optional>
#include <functional>

#include "solar/Utils.hpp"
#include "solar/core/UID.hpp"
#include "solar/system/System.hpp"
#include "solar/core/ExtendedSet.hpp"

namespace sr
{
	using SystemContainer = std::vector<std::unique_ptr<System>>;
	using ComponentContainer = std::vector<std::unique_ptr<SparseSet<Entity>>>;

	class Manager
	{
	public:
		Manager();
		~Manager();

		Entity create() noexcept;

		bool has(sr::Entity entity) noexcept;

		// check if an unsigned integer is an entity
		bool validate(sr::Entity entity);

		template<typename Component, typename... Args>
		void add(Entity entity, Args&&... args);

		template<typename Component>
		Component* get(Entity entity);

		template<typename... Components>
		decltype(auto) multi(Entity entity) noexcept;

		template<typename... Components>
		void operate(std::function<void(Entity, Components* ...)> lambda);

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
		void operateInteral(std::vector<Entity>& entities, unsigned int& counter);

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
	inline void Manager::add(Entity entity, Args&& ...args)
	{
		if (!validate(entity))
		{
			throw std::logic_error("Entity: " + std::to_string(entity) + " does not have a valid entity flag.");
		}

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
	inline Component* Manager::get(Entity entity)
	{
		if (!validate(entity))
		{
			throw std::logic_error("Entity: " + std::to_string(entity) + " does not have a valid entity flag.");
		}

		auto type = cuid::uid<Component>();

		if (type > m_data.size())
		{
			throw std::out_of_range("Attempted to access a component type that doesnt exist!");
		}
		
		ExtendedSet<Component>* derived = static_cast<ExtendedSet<Component>*>(m_data[cuid::uid<Component>()].get());
		return derived->get(entity);
	}

	template<typename... Components>
	inline decltype(auto) Manager::multi(Entity entity) noexcept
	{
		// get() will validate entity.
		return std::make_tuple(get<Components>(entity)...);
	}

	template<typename ...Components>
	inline void Manager::operate(std::function<void(Entity, Components* ...)> lambda)
	{
		// how many times operate internal is called.
		unsigned int counter = 0;
		
		std::vector<Entity> entities;
		entities.clear();
		(operateInteral<Components>(entities, counter), ...);

		if (counter > 1)
		{
			entities = Utils::findDuplicates(entities, counter);
		}

		// TODO: FIX ISSUE WHERE WHEN GETTING MULTIPLE COMPONENTS,
		// COMPONENTS FROM A AND B BUT NOT A AND B ARE being used.
		// I.e. when a,b is target, getting a but has no b.
		// also duplicates

		/*
			
			get all components belonging to the types and only keep entities that match all components and destroy
			the others that are not needed

			if entity is in type array a b and c etc]

			use a counter to see how many occurancers of an entity there shouild be

			https://www.google.com/search?client=firefox-b-d&ei=qvUuXYuwK6zZz7sP9tG50AI&q=C%2B%2B+checking+for+occurances+of+an+entity+in+a+set+of+arrays&oq=C%2B%2B+checking+for+occurances+of+an+entity+in+a+set+of+arrays&gs_l=psy-ab.3..33i10i21.2051.5564..5758...0.0..0.293.4290.2-18......0....1..gws-wiz.......0i71j33i10i160j33i22i29i30j33i10.7KmPP2JGx6Y

			https://www.google.com/search?client=firefox-b-d&q=check+if+an+integer+is+in+a+variable+amount+of+std%3A%3Avectors
		*/
			
		for (auto& entity : entities)
		{
			lambda(entity, get<Components>(entity)...);
		}
	}

	template<typename Component>
	inline void Manager::operateInteral(std::vector<Entity>& entities, unsigned int& counter)
	{
		auto type = cuid::uid<Component>();

		if (type > m_data.size())
		{
			throw std::out_of_range("Attempted to access a component type that doesnt exist!");
		}

		ExtendedSet<Component>* derived = static_cast<ExtendedSet<Component>*>(m_data[type].get());

		for (auto& e : derived->m_dense)
		{
			if (validate(e))
			{
				entities.push_back(e);
			}
		}

		counter++;
	}

	template<typename System, typename ...Args>
	inline void Manager::add(Args&&... args)
	{
		auto type = suid::uid<System>();
		if (type > m_systems.size())
		{
			m_systems.resize(type + 1);
		}

		m_systems[type] = std::make_unique<System>(std::forward<Args>(args)...);
	}

	template<typename System>
	inline System* Manager::get()
	{
		auto type = suid::uid<System>();
		if (type > m_systems.size())
		{
			throw std::out_of_range("Attempted to access a system type that doesnt exist!");
		}

		return m_systems[type].get();
	}
}

#endif