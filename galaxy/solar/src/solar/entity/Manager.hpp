///
/// Manager.hpp
///
/// solar
/// See LICENSE.txt.
///

#ifndef SOLAR_MANAGER_HPP_
#define SOLAR_MANAGER_HPP_

#include <span>
#include <memory>
#include <optional>
#include <unordered_set>

#include <protostar/system/UniqueID.hpp>
#include <pulsar/Log.hpp>
#include <range/v3/all.hpp>

#include "solar/system/System.hpp"
#include "solar/sets/ComponentSet.hpp"

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
	using CUniqueID = pr::UniqueID<struct ComponentUniqueID>;

	///
	/// Predefinition of unique id structure for systems.
	///
	using SUniqueID = pr::UniqueID<struct SystemUniqueID>;

	///
	/// Concept to ensure a system is actually derived from sr::System.
	///
	template<typename Type>
	concept is_system = (std::is_base_of_v<sr::System, Type> && std::is_convertible_v<const volatile Type*, const volatile sr::System*>);

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
		/// Copy constructor.
		///
		Manager(const Manager&) noexcept = delete;

		///
		/// Move constructor.
		///
		Manager(Manager&&) noexcept = delete;

		///
		/// Copy assignment operator.
		///
		Manager& operator=(const Manager&) noexcept = delete;

		///
		/// Move assignment operator.
		///
		Manager& operator=(Manager&&) noexcept = delete;

		///
		/// Destructor.
		///
		virtual ~Manager();

		///
		/// Create an entity.
		///
		/// \return An entity with a creation bit flag.
		///
		[[maybe_unused]] const sr::Entity create();

		///
		/// Create an entity with a name.
		///
		/// \param debug_name Debug name for identification purposes.
		///
		/// \return An entity with a creation bit flag.
		///
		[[maybe_unused]] const sr::Entity create(std::string_view debug_name);

		///
		/// Check if an entity exists.
		///
		/// \param entity Entity to verify.
		///
		/// \return True if entity does exist.
		///
		[[nodiscard]] const bool has(const sr::Entity entity) noexcept;

		///
		/// Check if an unsigned integer is an entity.
		/// Specifically checking for an sr::VALID_ENTITY bitflag.
		///
		/// \param uint Unsigned integer to verify.
		///
		/// \return True if unsigned integer is an entity.
		///
		[[nodiscard]] const bool validate(sr::Entity uint);

		///
		/// Assign a name to an entity.
		///
		/// \param entity Entity to assign name to.
		/// \param debug_name Name to assign to entity.
		///
		/// \return True if successful.
		///
		[[maybe_unused]] bool assign_name(const sr::Entity entity, std::string_view debug_name);

		///
		/// Get entity fropm debug name.
		///
		/// \param debugName Name of entity to retrieve.
		///
		/// \return The entity.
		///
		[[nodiscard]] sr::Entity find_from_name(std::string_view debugName);

		///
		/// Get all debug names in unordered_map.
		///
		/// \return Const unordered_map reference.
		///
		auto get_debug_name_map() noexcept -> const std::unordered_map<std::string, sr::Entity>&;

		///
		/// Add (construct) a component for an entity.
		/// Use template to specify type of component being created.
		///
		/// \param entity Entity to assosiate the component with.
		/// \param args Constructor arguments for the component.
		///
		/// \return Pointer to newly added component.
		///
		template<pr::is_class Component, typename... Args>
		Component* create_component(const sr::Entity entity, Args&&... args);

		///
		/// Retrieve a component assosiated with an entity.
		/// Template type is type of component to get.
		///
		/// \param entity Entity component is assosiated with.
		///
		/// \return Pointer to component of type Component.
		///
		template<pr::is_class Component>
		Component* get(const sr::Entity entity);

		///
		/// Remove a component assosiated with an entity.
		/// Template type is type of component to remove.
		///
		/// \param entity Entity component is assosiated with.
		///
		template<pr::is_class Component>
		void remove(const sr::Entity entity);

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
		template<pr::is_class... Components, typename Lambda>
		void operate(Lambda&& func);

		///
		/// \brief Add a system to the manager.
		///
		/// Template parameter to speficy type of system to create.
		///
		/// \param args Constructor arguments for the system.
		///
		template<is_system System, typename... Args>
		void create_system(Args&&... args);

		///
		/// Get a system. Type is template parameter.
		///
		/// \return Pointer to the system.
		///
		template<is_system System>
		System* get();

		///
		/// Destroys an entity and all associated components.
		///
		/// \param entity Entity to destroy.
		///
		void destroy(const sr::Entity entity);

		///
		/// Process system events.
		///
		void events();

		///
		/// Update all systems.
		///
		/// \param dt "Lag" time to pass to systems.
		///
		void update(const double dt);

		///
		/// Clear all data from Manager and reset.
		///
		void clear();

	protected:
		///
		/// Called by operate().
		///
		template<pr::is_class Component>
		void internal_operate(std::vector<sr::Entity>& entities);

		///
		/// Counter for free entity ids.
		///
		SR_INTEGER m_next_id;

		///
		/// Stores entitys.
		///
		EntitySet<sr::Entity> m_entities;

		///
		/// Stores invalid entities.
		///
		std::vector<sr::Entity> m_invalid_entities;

		///
		/// Debug entity names.
		///
		std::unordered_map<std::string, sr::Entity> m_debug_names;

		///
		/// Stores polymorphic ComponentSets.
		///
		ComponentContainer m_data;

		///
		/// Stores systems.
		///
		SystemContainer m_systems;
	};

	template<pr::is_class Component, typename... Args>
	inline Component* Manager::create_component(const sr::Entity entity, Args&&... args)
	{
		const auto type = CUniqueID::get<Component>();
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
			auto* derived = dynamic_cast<ComponentSet<Component>*>(m_data[type].get());
			if (derived)
			{
				if (!derived->has(entity))
				{
					return derived->create(entity, std::forward<Args>(args)...);
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

	template<pr::is_class Component>
	inline Component* Manager::get(const sr::Entity entity)
	{
		Component* res = nullptr;

		if (!validate(entity))
		{
			PL_LOG(PL_ERROR, "Attempted to get a component of an invalid entity.");
			res = nullptr;
		}
		else
		{
			const auto type = CUniqueID::get<Component>();

			if (type >= m_data.size() || m_data.size() == 0)
			{
				PL_LOG(PL_ERROR, "Attempted to access a component type that doesnt exist.");
				PL_LOG(PL_ERROR, "Possible zero size component data detected.");
			}
			else
			{
				if (m_data[type] != nullptr)
				{
					auto* derived = dynamic_cast<ComponentSet<Component>*>(m_data[type].get());
					if (derived->has(entity))
					{
						res = derived->get(entity);
					}
				}
			}
		}

		return res;
	}

	template<pr::is_class Component>
	inline void Manager::remove(const sr::Entity entity)
	{
		if (!validate(entity))
		{
			PL_LOG(PL_ERROR, "Attempted to get a component of an invalid entity.");
		}
		else
		{
			const auto type = CUniqueID::get<Component>();

			if (type >= m_data.size() || m_data.size() == 0)
			{
				PL_LOG(PL_ERROR, "Attempted to access a component type that doesnt exist.");
				PL_LOG(PL_ERROR, "Possible zero size component data detected.");
			}
			else
			{
				if (m_data[type] != nullptr)
				{
					auto* derived = dynamic_cast<ComponentSet<Component>*>(m_data[type].get());
					if (derived->has(entity))
					{
						derived->remove(entity);
					}
				}
			}
		}
	}

	template<pr::is_class... Components, typename Lambda>
	inline void Manager::operate(Lambda&& func)
	{
		// Ensure data is not empty.
		if (!m_data.empty())
		{
			constexpr auto length = sizeof...(Components);
			std::vector<sr::Entity> entities;

			// using ComponentContainer = std::vector<std::unique_ptr<EntitySet<sr::Entity>>>;
			(internal_operate<Components>(entities), ...);

			// So for all elements in the vector, filter by count entites predicate.
			auto filtered = entities | ranges::views::filter([&](sr::Entity e) {
						return true;
					});

			for (const sr::Entity e : filtered)
			{
				//func(e, (get<Components>(e), ...));
			}

			/*
			for (const sr::Entity e : ranges::views::all(entities) | ranges::views::filter(CountEntitiesPredicate {length, entities}()))
			std::vector<sr::Entity> filtered;
			for (const sr::Entity e : entities)
			{
				const auto count = std::count(entities.begin(), entities.end(), e);
				if (count == length)
				{
					if (std::find(filtered.begin(), filtered.end(), e) != filtered.end())
					{
						filtered.push_back(e);
					}
				}
			}



			std::vector<sr::Entity> selected;
			std::for_each(entities.begin(), entities.end(), p());
			*/
		}
	}

	template<pr::is_class Component>
	inline void Manager::internal_operate(std::vector<sr::Entity>& entities)
	{
		const auto type = CUniqueID::get<Component>();

		if (type >= m_data.size())
		{
			PL_LOG(PL_ERROR, "Attempted to access a component type that doesnt exist!");
		}
		else
		{
			auto ptr = m_data[type].get();
			if (ptr != nullptr)
			{
				entities.insert(entities.end(), ptr->m_dense.begin(), ptr->m_dense.end());
			}
		}
	}

	template<is_system System, typename... Args>
	inline void Manager::create_system(Args&&... args)
	{
		const auto type = SUniqueID::get<System>();
		if (type >= m_systems.size())
		{
			m_systems.resize(type + 1);
		}

		m_systems[type] = std::make_unique<System>(std::forward<Args>(args)...);
	}

	template<is_system System>
	inline System* Manager::get()
	{
		const auto type = SUniqueID::get<System>();
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
} // namespace sr

#endif