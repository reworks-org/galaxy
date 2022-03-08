///
/// World.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_WORLD_HPP_
#define GALAXY_CORE_WORLD_HPP_

#include <bitset>
#include <execution>
#include <optional>

#include "galaxy/ecs/ComponentSet.hpp"
#include "galaxy/ecs/System.hpp"
#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/Renderables.hpp"
#include "galaxy/meta/UniqueID.hpp"

namespace galaxy
{
	namespace core
	{
		class Scene2D;

		using CUniqueID           = meta::UniqueID<struct ComponentUniqueID>;
		using SUniqueID           = meta::UniqueID<struct SystemUniqueID>;
		using EntitysWithCounters = robin_hood::unordered_flat_map<ecs::Entity, int>;
		using ComponentFactory    = robin_hood::unordered_node_map<std::string, std::function<void(const ecs::Entity, const nlohmann::json&)>>;
		using Relationships       = robin_hood::unordered_flat_map<ecs::Entity, std::vector<ecs::Entity>>;

		///
		/// Concept to ensure a system is actually derived from a System.
		///
		template<typename Type>
		concept is_system = std::derived_from<Type, ecs::System>;

		///
		/// Manages the entities and systems and other library stuff, like the main lua state,
		/// and the physics world.
		///
		class World final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			World();

			///
			/// Destructor.
			///
			~World();

			///
			/// Update all registered systems.
			///
			/// \param scene Pointer to scene.
			///
			void update(core::Scene2D* scene);

			///
			/// Create an entity.
			///
			/// \return An entity with a creation bit flag.
			///
			[[maybe_unused]] const ecs::Entity create();

			///
			/// \brief Create an entity from a JSON file.
			///
			/// If your using this make sure you have called register_component().
			///
			/// \param file Filepath to the json.
			///
			/// \return Created entity.
			///
			[[maybe_unused]] std::optional<ecs::Entity> create_from_json(std::string_view file);

			///
			/// \brief Create an entity from a JSON object.
			///
			/// If your using this make sure you have called register_component().
			///
			/// \param json Preloaded JSON object.
			///
			/// \return Created entity.
			///
			[[maybe_unused]] const ecs::Entity create_from_json_obj(const nlohmann::json& json);

			///
			/// Set a flag on an entity.
			///
			/// \param entity Entity to set flag on.
			///
			template<meta::is_bitset_flag Flag>
			void set_flag(const ecs::Entity entity);

			///
			/// Check flag value on an entity.
			///
			/// \param entity Entity to check flag.
			///
			template<meta::is_bitset_flag Flag>
			[[nodiscard]] const bool is_flag_set(const ecs::Entity entity);

			///
			/// Unset a flag on an entity.
			///
			/// \param entity Entity to unset flag on.
			///
			template<meta::is_bitset_flag Flag>
			void unset_flag(const ecs::Entity entity);

			///
			/// Registers a component definition.
			///
			/// \param name Name of component class in string format i.e. "graphics::AnimatedBatchSprite".
			///
			template<meta::is_class Component>
			void register_component(std::string_view name);

			///
			/// Add (construct) a component for an entity.
			/// Use template to specify type of component being created.
			///
			/// \param entity Entity to assossiate the component with.
			/// \param args Constructor arguments for the component.
			///
			/// \return Pointer to newly added component.
			///
			template<meta::is_class Component, typename... Args>
			[[maybe_unused]] Component* create_component(const ecs::Entity entity, Args&&... args);

			///
			/// Retrieve a component assosiated with an entity.
			/// Template type is type of component to get.
			///
			/// \param entity Entity component is assosiated with.
			///
			/// \return Pointer to component of type Component.
			///
			template<meta::is_class Component>
			[[nodiscard]] Component* get(const ecs::Entity entity);

			///
			/// Set an entity as the parent of a relationship.
			///
			/// \param entity Entity to make a parent.
			///
			void make_parent(const ecs::Entity entity) noexcept;

			///
			/// Check if an entity is a parent.
			///
			/// \return True if entity is a parent.
			///
			[[nodiscard]] const bool is_parent(const ecs::Entity entity) noexcept;

			///
			/// Removes a parent and all its children.
			///
			/// \param parent Parent entity of a relationship.
			///
			void remove_parent(const ecs::Entity parent) noexcept;

			///
			/// Destroy a parent entity and all its children.
			///
			/// \param parent Parent entity of a relationship.
			///
			void destroy_parent(const ecs::Entity parent) noexcept;

			///
			/// Assign an entity to a parent as a child.
			///
			/// \param parent Parent entity of a relationship.
			/// \param child Child entity to form a relationship with a parent.
			///
			void assign_child(const ecs::Entity parent, const ecs::Entity child) noexcept;

			///
			/// Remove a child entity from a parent entity.
			///
			/// \param parent Parent entity of a relationship.
			/// \param child Child entity to remove.
			///
			void remove_child(const ecs::Entity parent, const ecs::Entity child) noexcept;

			///
			/// Get all children of a parent entity.
			///
			/// \return Pointer to vector of childen, or nullptr if not parent. Will be empty if no children, not nullptr.
			///
			[[nodiscard]] std::vector<ecs::Entity>* const get_children(const ecs::Entity parent) noexcept;

			///
			/// Remove a component assosiated with an entity.
			/// Template type is type of component to remove.
			///
			/// \param entity Entity component is assosiated with.
			///
			template<meta::is_class Component>
			void remove(const ecs::Entity entity);

			///
			/// \brief Destroys an entity and all associated components.
			///
			/// If the entity is a parent this will destroy the relationship but not the children.
			///
			/// \param entity Entity to destroy.
			///
			void destroy(const ecs::Entity entity);

			///
			/// Check if an entity exists.
			///
			/// \param entity Entity to verify.
			///
			/// \return True if entity does exist.
			///
			[[nodiscard]] const bool has(const ecs::Entity entity) noexcept;

			///
			/// Check if an entity is enabled.
			///
			/// \param entity Entity to check.
			///
			/// \return True if entity is enabled.
			///
			[[nodiscard]] const bool is_enabled(const ecs::Entity entity);

			///
			/// Query how many entities belong to a component.
			///
			/// \return Const unsigned int. Only returns enabled entities.
			///
			template<meta::is_class Component>
			[[nodiscard]] const unsigned int query_count();

			///
			/// Enable an entity.
			///
			/// \param entity Entity to enable.
			///
			void enable(const ecs::Entity entity);

			///
			/// Disable an entity.
			///
			/// \param entity Entity to disable.
			///
			void disable(const ecs::Entity entity);

			///
			/// Retrieve multiple components assosiated with an entity.
			/// Template type is type of components to get.
			///
			/// \param entity Entity components are assosiated with.
			///
			/// \return Tuple of pointers to components.
			///
			template<meta::is_class... Component>
			[[nodiscard]] std::tuple<Component*...> get_multi(const ecs::Entity entity);

			///
			/// \brief Iterate over a set of components of a set of types and manipulate their data.
			///
			/// The components to manipulate are specified in the template parameter.
			///
			/// \param func A lambda function that manipulates the components.
			///		          For example:
			/*
								manager.operate<a, b>([](const ecs::Entity entity, a* ca, b* cb)
								{
									cb->var = 500;
								});
								*/
			///
			template<meta::is_class... Components, typename Lambda>
			void operate(Lambda&& func);

			///
			/// \brief Iterate over a set of components of a set of types and manipulate their data.
			///
			/// The components to manipulate are specified in the template parameter.
			///
			/// \param policy STL exection policy. Beware that this may run on another thread.
			/// \param func A lambda function that manipulates the components.
			///		          For example:
			/*
								manager.operate<a, b>([](const ecs::Entity entity, a* ca, b* cb)
								{
									cb->var = 500;
								});
								*/
			///
			template<meta::is_class... Components, typename Policy, typename Lambda>
			void operate(Policy&& policy, Lambda&& func);

			///
			/// Apply a function to each entity.
			///
			/// \param func Lambda is required to take in a const ecs::Entity type.
			///
			template<typename Lambda>
			void each(Lambda&& func) const;

			///
			/// \brief Add a system to the manager.
			///
			/// Template parameter to speficy type of system to create.
			/// Systems will be updated in the order in which they are created.
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
			[[nodiscard]] System* get_system();

			///
			/// Clear all entity data from world.
			///
			void clear();

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to be serialized.
			///
			[[nodiscard]] nlohmann::json serialize() override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			void deserialize(const nlohmann::json& json) override;

		private:
			///
			/// Copy constructor.
			///
			World(const World&) = delete;

			///
			/// Move constructor.
			///
			World(World&&) = delete;

			///
			/// Copy assignment operator.
			///
			World& operator=(const World&) = delete;

			///
			/// Move assignment operator.
			///
			World& operator=(World&&) = delete;

			///
			/// Called by operate().
			///
			template<meta::is_class Component>
			void internal_operate(EntitysWithCounters& entities);

		private:
			///
			/// Counter for free entity ids.
			///
			GALAXY_ENTITY_SIZE m_next_id;

			///
			/// Stores entitys.
			///
			std::vector<ecs::Entity> m_entities;

			///
			/// Stores invalid entities.
			///
			std::vector<ecs::Entity> m_invalid_entities;

			///
			/// Stores entity flags.
			///
			robin_hood::unordered_flat_map<ecs::Entity, std::bitset<8>> m_flags;

			///
			/// Debug entity names.
			///
			robin_hood::unordered_flat_map<std::string, ecs::Entity> m_debug_names;

			///
			/// Stores polymorphic ComponentSets.
			///
			std::vector<std::unique_ptr<ecs::Set>> m_data;

			///
			/// Stores systems.
			///
			robin_hood::unordered_flat_map<std::size_t, std::unique_ptr<ecs::System>> m_systems;

			///
			/// Used to allow for component creation without having to know the compile time type.
			///
			ComponentFactory m_component_factory;

			///
			/// Holds relationships between "parent" and "child" entities.
			///
			Relationships m_relationships;
		};

		template<meta::is_bitset_flag Flag>
		inline void World::set_flag(const ecs::Entity entity)
		{
			if (m_flags.contains(entity))
			{
				m_flags[entity].set(Flag::value);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to set flag: {0} for entity: {1}. Entity does not exist.", Flag::value, entity);
			}
		}

		template<meta::is_bitset_flag Flag>
		inline const bool World::is_flag_set(const ecs::Entity entity)
		{
			if (m_flags.contains(entity))
			{
				return m_flags[entity].test(Flag::value);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to test flag: {0} for entity: {1}. Entity does not exist.", Flag::value, entity);
				return false;
			}
		}

		template<meta::is_bitset_flag Flag>
		inline void World::unset_flag(const ecs::Entity entity)
		{
			if (m_flags.contains(entity))
			{
				m_flags[entity].reset(Flag::value);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to unset flag: {0} for entity: {1}. Entity does not exist.", Flag::value, entity);
			}
		}

		template<meta::is_class Component>
		inline void World::register_component(std::string_view name)
		{
			// Make sure there are no duplicate components for the hashmap before registering.
			const auto str = static_cast<std::string>(name);
			if (m_component_factory.contains(str))
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to register duplicate component: {0}.", str);
			}
			else
			{
				m_component_factory.emplace(name, [&](const ecs::Entity e, const nlohmann::json& json) {
					create_component<Component>(e, json);
				});
			}
		}

		template<meta::is_class Component, typename... Args>
		inline Component* World::create_component(const ecs::Entity entity, Args&&... args)
		{
			if (has(entity))
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
						m_data[type] = std::make_unique<ecs::ComponentSet<Component>>();
					}

					// Now convert the storage to the type we want to access.
					auto* derived = static_cast<ecs::ComponentSet<Component>*>(m_data[type].get());
					if (derived)
					{
						if (!derived->has(entity))
						{
							return derived->create(entity, std::forward<Args>(args)...);
						}
						else
						{
							GALAXY_LOG(GALAXY_WARNING, "Attempted to add a duplicate component.");
							return nullptr;
						}
					}
					else
					{
						return nullptr;
					}
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to create a component with an invalid entity.");
				return nullptr;
			}
		}

		template<meta::is_class Component>
		inline Component* World::get(const ecs::Entity entity)
		{
			Component* res = nullptr;

			const auto type = CUniqueID::get<Component>();

			if (!(type >= m_data.size() || m_data.size() == 0))
			{
				if (m_data[type] != nullptr)
				{
					auto* derived = static_cast<ecs::ComponentSet<Component>*>(m_data[type].get());
					if (derived->has(entity))
					{
						res = derived->get(entity);
					}
				}
			}

			return res;
		}

		template<meta::is_class Component>
		inline void World::remove(const ecs::Entity entity)
		{
			const auto type = CUniqueID::get<Component>();

			if (type >= m_data.size() || m_data.size() == 0)
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to remove a component that doesnt exist.");
			}
			else
			{
				if (m_data[type] != nullptr)
				{
					auto* derived = static_cast<ecs::ComponentSet<Component>*>(m_data[type].get());
					derived->remove(entity);
				}
			}
		}

		template<meta::is_class Component>
		inline const unsigned int World::query_count()
		{
			const auto type = CUniqueID::get<Component>();

			if (!(type >= m_data.size() || m_data.size() == 0))
			{
				if (m_data[type] != nullptr)
				{
					return m_data[type]->get_size();
				}
			}

			return 0;
		}

		template<meta::is_class... Component>
		inline std::tuple<Component*...> World::get_multi(const ecs::Entity entity)
		{
			return std::make_tuple<Component*...>(get<Component>(entity)...);
		}

		template<meta::is_class... Components, typename Lambda>
		inline void World::operate(Lambda&& func)
		{
			constexpr const auto length = sizeof...(Components);
			EntitysWithCounters  entities;

			(internal_operate<Components>(entities), ...);

			for (const auto& [entity, count] : entities)
			{
				// Ensures that only entities that have all components are used.
				if (!(count < length))
				{
					if (is_enabled(entity))
					{
						func(entity, get<Components>(entity)...);
					}
				}
			}
		}

		template<meta::is_class... Components, typename Policy, typename Lambda>
		inline void World::operate(Policy&& policy, Lambda&& func)
		{
			constexpr const auto length = sizeof...(Components);
			EntitysWithCounters  entities;

			(internal_operate<Components>(entities), ...);

			// pair.first = entity
			// pair.second = count
			std::for_each(policy, entities.begin(), entities.end(), [&](const auto& pair) {
				// Ensures that only entities that have all components are used.
				if (!(pair.second < length))
				{
					if (is_enabled(pair.first))
					{
						func(pair.first, get<Components>(pair.first)...);
					}
				}
			});
		}

		template<typename Lambda>
		inline void World::each(Lambda&& func) const
		{
			for (const auto& entity : m_entities)
			{
				func(entity);
			}
		}

		template<is_system System, typename... Args>
		inline void World::create_system(Args&&... args)
		{
			const auto type = SUniqueID::get<System>();
			m_systems.emplace(type, std::make_unique<System>(std::forward<Args>(args)...));
		}

		template<is_system System>
		inline System* World::get_system()
		{
			const auto type = SUniqueID::get<System>();

			if (m_systems.contains(type))
			{
				return static_cast<System*>(m_systems[type].get());
			}
			else
			{
				GALAXY_LOG(GALAXY_FATAL, "Attempted to access a system type that doesnt exist!");
				return nullptr;
			}
		}

		template<meta::is_class Component>
		inline void World::internal_operate(EntitysWithCounters& entities)
		{
			const auto type = CUniqueID::get<Component>();
			if (!(type >= m_data.size()))
			{
				if (m_data[type].get())
				{
					for (const auto& entity : m_data[type].get()->m_entities)
					{
						entities[entity]++;
					}
				}
			}
		}
	} // namespace core
} // namespace galaxy

#endif