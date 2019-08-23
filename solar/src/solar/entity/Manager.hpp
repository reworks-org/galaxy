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

	///
	/// Controls and manages the Entitys, Components and Systems.
	///
	class Manager
	{
	public:
		///
		/// Constructor.
		///
		Manager();

		/// Destructor.
		///
		///
		~Manager();

		///
		/// Create an entity.
		///
		/// \return An entity with a creation bit flag.
		///
		Entity create() noexcept;

		///
		/// Check if an entity exists.
		///
		/// \param entity Entity to verify.
		///
		/// \return True if entity does exist.
		///
		bool has(sr::Entity entity) noexcept;

		///
		/// Check if an unsigned integer is an entity.
		/// Specifically checking for an sr::VALID_ENTITY bitflag.
		///
		/// \param uint Unsigned integer to verify.
		///
		/// \return True if unsigned integer is an entity.
		///
		bool validate(sr::Entity uint);

		///
		/// Add (construct) a component for an entity.
		/// Use template to specify type of component being created.
		///
		/// \param entity Entity to assosiate the component with.
		/// \param args Constructor arguments for the component.
		///
		template<typename Component, typename... Args>
		void add(Entity entity, Args&&... args);

		///
		/// Retrieve a component assosiated with an entity.
		/// Template type is type of component to get.
		///
		/// \param entity Entity component is assosiated with.
		///
		/// \return Pointer to component of type Component.
		///
		template<typename Component>
		Component* get(Entity entity);

		///
		/// \brief Retrieve multiple components.
		///
		/// Using the template parameter, specify the component(s) you want to retrieve.
		/// Use structured binding. auto [x, y] = multi<x, y>(entity); 
		/// The first value in the binding is the first type in the template parameter.
		///
		/// \param entity Entity that the components belong to.
		///
		/// \return Type is automatically deduced, but is a type of std::tuple. Best method is to use structured bindings to retrieve data.
		///
		template<typename... Components>
		decltype(auto) multi(Entity entity) noexcept;

		///
		/// \brief Iterate over a set of components of a set of types and manipulate their data.
		///
		/// The components to manipulate are specified in the template parameter.
		///
		/// \param lambda A lambda function that manipulates the components.
		///		          For example: 
							/*
							manager.operate<a, b>([](sr::Entity entity, a* ca, b* cb)
							{
								cb->var = 500;
							});
							*/
		///
		template<typename... Components>
		void operate(std::function<void(Entity, Components* ...)> lambda);

		///
		/// \brief Add a system to the manager.
		///
		/// Template parameter to speficy type of system to create.
		///
		/// \param args Constructor arguments for the system.
		///
		template<typename System, typename... Args>
		void add(Args&&... args);

		///
		/// Get a system. Type is template parameter.
		///
		/// \return Pointer to the system.
		///
		template<typename System>
		System* get();

		///
		/// Destroys an entity and all assosiated components.
		///
		/// \param entity Entity to destroy.
		///
		void destroy(Entity entity);

		///
		/// Pass on event to all systems.
		///
		/// \param event Event data to pass to systems.
		///
		void event(const Event& event);

		///
		/// Update all systems.
		///
		/// \param time DeltaTime to pass to systems.
		///
		void update(const DeltaTime time);

	private:
		///
		/// Internal method used to process components from entities.
		///
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

		///
		/// Stores systems.
		///
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
		// expands to be called on every component, also incrementing counter to know how many times called.
		(operateInteral<Components>(entities, counter), ...);

		if (counter > 1)
		{
			// erase duplicates
			// TODO: find a more efficient method of matching.
			entities = Utils::findDuplicates(entities, counter);
		}
			
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
			// Have to make sure no entitys are blank unsigned integers.
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
		if (type >= m_systems.size())
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

		return static_cast<System*>(m_systems[type].get());
	}
}

#endif