///
/// Manager.hpp
///
/// solar
/// See LICENSE.txt.
///

#ifndef SOLAR_MANAGER_HPP_
#define SOLAR_MANAGER_HPP_

#include <memory>
#include <optional>
#include <unordered_set>

#include <pulsar/Log.hpp>
#include <protostar/math/Maths.hpp>

#include "solar/system/System.hpp"
#include "solar/sets/ComponentSet.hpp"
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
	using ComponentContainer = std::vector<std::unique_ptr<EntitySet<sr::Entity>>>;

	///
	/// Predefinition of unique id structure for components.
	///
	using cUniqueID = pr::UniqueID<struct ComponentUniqueID>;

	///
	/// Predefinition of unique id structure for systems.
	///
	using sUniqueID = pr::UniqueID<struct SystemUniqueID>;

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
		/// Create an entity with a name.
		///
		/// \param debugName Debug name for identification purposes.
		///
		/// \return An entity with a creation bit flag.
		///
		const sr::Entity create(const std::string& debugName) noexcept;

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
		/// Assign a name to an entity.
		///
		/// \param entity Entity to assign name to.
		/// \param debugName Name to assign to entity.
		///
		/// \return True if successful.
		///
		bool assignName(const sr::Entity entity, const std::string& debugName) noexcept;

		///
		/// Get entity fropm debug name.
		///
		/// \param debugName Name of entity to retrieve.
		///
		/// \return The entity.
		///
		sr::Entity findFromName(const std::string& debugName) noexcept;

		///
		/// Get all debug names in unordered_map. 
		///
		/// \return Const unordered_map reference.
		///
		const std::unordered_map<std::string, sr::Entity>& getAllNames() noexcept;

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
		/// Remove a component assosiated with an entity.
		/// Template type is type of component to remove.
		///
		/// \param entity Entity component is assosiated with.
		///
		template<typename Component>
		void remove(const sr::Entity entity) noexcept;

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
		void update(pr::ProtectedDouble* deltaTime) noexcept;

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
		void iOperate(std::list<std::vector<sr::Entity>*>& entities) noexcept;

	protected:
		///
		/// Counter for free entity ids.
		///
		SR_INTEGER m_nextID;

		///
		/// Stores entitys.
		///
		EntitySet<sr::Entity> m_entities;

		///
		/// Stores invalid entities.
		///
		std::vector<sr::Entity> m_invalidEntities;

		///
		/// Debug entity names.
		///
		std::unordered_map<std::string, sr::Entity> m_debugNames;

		///
		/// Stores polymorphic ComponentSets.
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
		const auto type = cUniqueID::get<Component>();
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
				m_data[type] = std::make_unique<ComponentSet<Component>>();
			}

			// Now convert the storage to the type we want to access.
			ComponentSet<Component>* derived = dynamic_cast<ComponentSet<Component>*>(m_data[type].get());
			if (derived)
			{
				if (!derived->has(entity))
				{
					return derived->add(entity, std::forward<Args>(args)...);
				}
				else
				{
					PL_LOG(PL_WARNING, "Attempted to add a duplicate component.");
					return nullptr;
				}
			}
			else
			{
				return nullptr;
			}
		}
	}

	template<typename Component>
	inline Component* Manager::get(const sr::Entity entity) noexcept
	{
		Component* res = nullptr;

		if (!validate(entity))
		{
			PL_LOG(PL_ERROR, "Attempted to get a component of an invalid entity.");
			res = nullptr;
		}
		else
		{
			const auto type = cUniqueID::get<Component>();

			if (type >= m_data.size() || m_data.size() == 0)
			{
				PL_LOG(PL_ERROR, "Attempted to access a component type that doesnt exist.");
				PL_LOG(PL_ERROR, "Possible zero size component data detected.");
			}
			else
			{
				if (m_data[type] != nullptr)
				{
					ComponentSet<Component>* derived = dynamic_cast<ComponentSet<Component>*>(m_data[type].get());
					if (derived->has(entity))
					{
						res = derived->get(entity);
					}
				}
			}
		}

		return res;
	}

	template<typename Component>
	inline void Manager::remove(const sr::Entity entity) noexcept
	{
		if (!validate(entity))
		{
			PL_LOG(PL_ERROR, "Attempted to get a component of an invalid entity.");
		}
		else
		{
			const auto type = cUniqueID::get<Component>();

			if (type >= m_data.size() || m_data.size() == 0)
			{
				PL_LOG(PL_ERROR, "Attempted to access a component type that doesnt exist.");
				PL_LOG(PL_ERROR, "Possible zero size component data detected.");
			}
			else
			{
				if (m_data[type] != nullptr)
				{
					ComponentSet<Component>* derived = dynamic_cast<ComponentSet<Component>*>(m_data[type].get());
					if (derived->has(entity))
					{
						derived->remove(entity);
					}
				}
			}
		}
	}

	template<typename... Components, typename Lambda>
	inline void Manager::operate(Lambda&& lambda) noexcept
	{
		if (!m_data.empty())
		{
			// Gets expanded to be called for every parameter in component.
			std::list<std::vector<sr::Entity>*> all;
			(iOperate<Components>(all), ...);

			constexpr auto length = sizeof...(Components);
			if (length == all.size())
			{
				auto entities = pr::intersections<sr::Entity>(all);
				if (!entities.empty())
				{
					for (const auto& entity : entities)
					{
						lambda(entity, get<Components>(entity)...);
					}
				}
			}
		}
	}

	template<typename Component>
	inline void Manager::iOperate(std::list<std::vector<sr::Entity>*>& entities) noexcept
	{
		auto type = cUniqueID::get<Component>();

		if (type >= m_data.size())
		{
			PL_LOG(PL_ERROR, "Attempted to access a component type that doesnt exist!");
		}
		else
		{
			if (m_data[type] != nullptr)
			{
				ComponentSet<Component>* derived = static_cast<ComponentSet<Component>*>(m_data[type].get());
				entities.push_back(&derived->m_dense);
			}
		}
	}

	template<typename System, typename ...Args>
	inline void Manager::addSystem(Args&&... args) noexcept
	{
		const auto type = sUniqueID::get<System>();
		if (type >= m_systems.size())
		{
			m_systems.resize(type + 1);
		}

		m_systems[type] = std::make_unique<System>(std::forward<Args>(args)...);
	}

	template<typename System>
	inline System* Manager::get() noexcept
	{
		const auto type = sUniqueID::get<System>();
		if (type > m_systems.size())
		{
			PL_LOG(PL_FATAL, "Attempted to access a system type that doesnt exist!");
			return nullptr;
		}
		else
		{
			return dynamic_cast<System*>(m_systems[type].get());
		}
	}
}

#endif