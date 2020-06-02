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
#include <algorithm>
#include <unordered_set>

#include <pulsar/Log.hpp>

#include "solar/system/System.hpp"
#include "solar/detail/DualSparseSet.hpp"
#include "protostar/utility/UniqueID.hpp"

///
/// Core namespace.
///
namespace sr
{
	///
	/// Shorthand system storage type.
	///
	using SystemContainer = std::vector<std::unique_ptr<sr::System>>;

	///
	/// Shorthand component storage type.
	///
	using ComponentContainer = std::vector<std::unique_ptr<SparseSet<sr::Entity>>>;

	///
	/// Predefinition of unique id structure for components.
	///
	using cUniqueID = protostar::UniqueID<struct ComponentUniqueID>;

	///
	/// Predefinition of unique id structure for systems.
	///
	using sUniqueID = protostar::UniqueID<struct SystemUniqueID>;

	///
	/// Controls and manages the Entitys, Components and Systems.
	///
	class Manager
	{
	public:
		///
		/// Constructor.
		///
		Manager() noexcept;

		///
		/// Destructor.
		///
		virtual ~Manager() noexcept;

		///
		/// Create an entity.
		///
		/// \return An entity with a creation bit flag.
		///
		const sr::Entity create() noexcept;

		///
		/// Check if an entity exists.
		///
		/// \param entity Entity to verify.
		///
		/// \return True if entity does exist.
		///
		const bool has(const sr::Entity entity) noexcept;

		///
		/// Check if an unsigned integer is an entity.
		/// Specifically checking for an sr::VALID_ENTITY bitflag.
		///
		/// \param uint Unsigned integer to verify.
		///
		/// \return True if unsigned integer is an entity.
		///
		const bool validate(sr::Entity uint) noexcept;

		///
		/// Add (construct) a component for an entity.
		/// Use template to specify type of component being created.
		///
		/// \param entity Entity to assosiate the component with.
		/// \param args Constructor arguments for the component.
		///
		/// \return Pointer to newly added component.
		///
		template<typename Component, typename... Args>
		Component* add(const sr::Entity entity, Args&&... args) noexcept;

		///
		/// Retrieve a component assosiated with an entity.
		/// Template type is type of component to get.
		///
		/// \param entity Entity component is assosiated with.
		///
		/// \return Pointer to component of type Component.
		///
		template<typename Component>
		Component* get(const sr::Entity entity) noexcept;

		///
		/// \brief Iterate over a set of components of a set of types and manipulate their data.
		///
		/// The components to manipulate are specified in the template parameter.
		///
		/// \param lambda A lambda function that manipulates the components.
		///		          For example: 
							/*
							manager.operate<a, b>([](const sr::Entity entity, a* ca, b* cb)
							{
								cb->var = 500;
							});
							*/
		///
		template<typename... Components, typename Lambda>
		void operate(Lambda&& lambda) noexcept;

		///
		/// \brief Add a system to the manager.
		///
		/// Template parameter to speficy type of system to create.
		///
		/// \param args Constructor arguments for the system.
		///
		template<typename System, typename... Args>
		void addSystem(Args&&... args) noexcept;

		///
		/// Get a system. Type is template parameter.
		///
		/// \return Pointer to the system.
		///
		template<typename System>
		System* get() noexcept;

		///
		/// Destroys an entity and all assosiated components.
		///
		/// \param entity Entity to destroy.
		///
		void destroy(const sr::Entity entity) noexcept;

		///
		/// Process system events.
		///
		void events() noexcept;

		///
		/// Update all systems.
		///
		/// \param deltaTime "Lag" time to pass to systems.
		///
		void update(protostar::ProtectedDouble* deltaTime) noexcept;

		///
		/// Clear all data from Manager and reset.
		///
		void clear() noexcept;

	protected:
		///
		/// Internal method used to process components from entities.
		///
		/// \param entities Entitys to operate on.
		///
		template<typename Component>
		void iOperate(std::vector<sr::Entity>& entities) noexcept;

	protected:
		///
		/// Counter for free entity ids.
		///
		SR_INTEGER m_nextID;

		///
		/// Stores entitys.
		///
		SparseSet<sr::Entity> m_entities;

		///
		/// Stores polymorphic DualSparseSets.
		///
		ComponentContainer m_data;

		///
		/// Stores systems.
		///
		SystemContainer m_systems;
	};

	template<typename Component, typename... Args>
	inline Component* Manager::add(const sr::Entity entity, Args&& ...args) noexcept
	{
		if (!validate(entity))
		{
			PL_LOG(PL_FATAL, "Entity: " + std::to_string(entity) + " does not have a valid entity flag.");
			return nullptr;
		}
		else
		{
			auto type = cUniqueID::get<Component>();

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
					m_data[type] = std::make_unique<DualSparseSet<Component>>();
				}

				// Now convert the storage to the type we want to access.
				DualSparseSet<Component>* derived = static_cast<DualSparseSet<Component>*>(m_data[type].get());

				return derived->add(entity, std::forward<Args>(args)...);
			}
		}
	}

	template<typename Component>
	inline Component* Manager::get(const sr::Entity entity) noexcept
	{
		Component* res = nullptr;

		if (!validate(entity))
		{
			PL_LOG(PL_FATAL, "Entity: " + std::to_string(entity) + " does not have a valid entity flag.");
			res = nullptr;
		}
		else
		{
			auto type = cUniqueID::get<Component>();

			if (type > m_data.size() || m_data.size() == 0)
			{
				PL_LOG(PL_ERROR, "Attempted to access a component type that doesnt exist.");
				PL_LOG(PL_ERROR, "Possible zero size component data detected.");
			}
			else
			{
				DualSparseSet<Component>* derived = static_cast<DualSparseSet<Component>*>(m_data[type].get());
				res = derived->get(entity);
			}
		}

		return res;
	}

	template<typename... Components, typename Lambda>
	inline void Manager::operate(Lambda&& lambda) noexcept
	{
		if (!m_data.empty())
		{
			std::vector<sr::Entity> entities;

			// expands to be called on every component, also incrementing counter to know how many times called.
			(iOperate<Components>(entities), ...);

			// Erase any duplicates. This is faster than sort + unique apparently.
			std::unordered_set<sr::Entity> set;
			for (sr::Entity e : entities)
			{
				set.insert(e);
			}
			entities.assign(set.begin(), set.end());
			//std::sort(entities.begin(), entities.end());

			for (auto& entity : entities)
			{
				lambda(entity, get<Components>(entity)...);
			}
		}
	}

	template<typename Component>
	inline void Manager::iOperate(std::vector<Entity>& entities) noexcept
	{
		auto type = cUniqueID::get<Component>();

		if (type > m_data.size())
		{
			PL_LOG(PL_FATAL, "Attempted to access a component type that doesnt exist!");
		}
		else
		{
			DualSparseSet<Component>* derived = static_cast<DualSparseSet<Component>*>(m_data[type].get());

			for (auto& e : derived->m_dense)
			{
				// Have to make sure no entitys are blank unsigned integers.
				if (validate(e))
				{
					entities.push_back(e);
				}
			}
		}
	}

	template<typename System, typename ...Args>
	inline void Manager::addSystem(Args&&... args) noexcept
	{
		auto type = sUniqueID::get<System>();
		if (type >= m_systems.size())
		{
			m_systems.resize(type + 1);
		}

		m_systems[type] = std::make_unique<System>(std::forward<Args>(args)...);
	}

	template<typename System>
	inline System* Manager::get() noexcept
	{
		auto type = sUniqueID::get<System>();
		if (type > m_systems.size())
		{
			PL_LOG(PL_FATAL, "Attempted to access a system type that doesnt exist!");
			return nullptr;
		}
		else
		{
			return static_cast<System*>(m_systems[type].get());
		}
	}
}

#endif