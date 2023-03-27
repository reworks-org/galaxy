///
/// World.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_WORLD_HPP_
#define GALAXY_CORE_WORLD_HPP_

#include <typeindex>

#include <box2d/b2_world.h>
#include <entt/entity/registry.hpp>
#include <robin_hood.h>

#include "galaxy/error/Log.hpp"
#include "galaxy/fs/Serializable.hpp"
#include "galaxy/meta/Concepts.hpp"
#include "galaxy/systems/RenderSystem.hpp"

namespace galaxy
{
	namespace components
	{
		class RigidBody;
		class Transform;
	} // namespace components

	namespace scene
	{
		class Scene;
		class RuntimeLayer;
	} // namespace scene

	namespace core
	{
		///
		/// Provides an additional set of functionality around an entt::registry.
		///
		class World final : public fs::Serializable
		{
			using SystemContainer  = std::vector<std::shared_ptr<systems::System>>;
			using ComponentFactory = robin_hood::unordered_flat_map<std::string, std::function<void(const entt::entity, const nlohmann::json&)>>;

		public:
			///
			/// Constructor.
			///
			/// \param scene Non-owning pointer to current scene.
			///
			World(scene::Scene* scene);

			///
			/// Destructor.
			///
			~World();

			///
			/// \brief Create an entity with some default components.
			///
			/// \return Created entity, or entt::null if failed.
			///
			[[maybe_unused]] entt::entity create();

			///
			/// \brief Create an entity from a prefab.
			///
			/// If your using this make sure you have called register_component().
			///
			/// \param name Name of the prefab to load.
			///
			/// \return Created entity, or entt::null if failed.
			///
			[[maybe_unused]] entt::entity create_from_prefab(const std::string& name);

			///
			/// \brief Create an entity from a JSON object.
			///
			/// If your using this make sure you have called register_component().
			///
			/// \param json Preloaded JSON object.
			///
			/// \return Created entity, or entt::null if failed.
			///
			[[maybe_unused]] entt::entity create_from_json(const nlohmann::json& json);

			///
			/// Defines a dependency validation for components.
			///
			template<meta::valid_component ToValidate, meta::valid_component... Dependencies>
			void register_dependencies();

			///
			/// Validate an entity to make sure all components have met their requirements as defined by register_dependencies().
			///
			/// \return True if entity is valid.
			///
			[[nodiscard]] bool is_valid(const entt::entity entity);

			///
			/// Registers a component definition.
			///
			/// \tparam Component A valid component type is required. Must be move assignable/constructible and a class with a json constructor.
			///
			/// \param name Name of component class in string format i.e. "Transform" or "Tag".
			///
			template<meta::valid_component Component>
			void register_component(const std::string& name);

			///
			/// \brief Add a system to the manager.
			///
			/// \tparam System Type of system to create.
			/// \tparam Args Constructor arguments for system.
			///
			/// Systems will be updated in the order in which they are created.
			///
			/// \param args Constructor arguments for system.
			///
			/// \return Weak pointer to the system.
			///
			template<meta::is_system System, typename... Args>
			[[maybe_unused]] std::weak_ptr<System> create_system(Args&&... args);

			///
			/// Update world data.
			///
			void update();

			///
			/// Only update the rendersystem.
			///
			void update_rendersystem();

			///
			/// Clears all system, component registry, and entity data.
			///
			void clear();

			///
			/// Serialise a single entity.
			///
			/// \param entity Entity to serialize.
			///
			/// \return JSON entity data.
			///
			[[nodiscard]] nlohmann::json serialize_entity(const entt::entity entity);

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
			/// Function that integrates lua init with entt on construct event.
			///
			/// \param registry Registry component belongs to.
			/// \param entity Entity component belongs to.
			///
			void construct_script(entt::registry& registry, entt::entity entity);

			///
			/// Function that integrates lua destroy with entt on destruction event.
			///
			/// \param registry Registry component belongs to.
			/// \param entity Entity component belongs to.
			///
			void destruct_script(entt::registry& registry, entt::entity entity);

			///
			/// Function that integrates a box2d construction with entt.
			///
			/// \param registry Registry component belongs to.
			/// \param entity Entity component belongs to.
			///
			void construct_rigidbody(entt::registry& registry, entt::entity entity);

			///
			/// Function that integrates a box2d destruction with entt.
			///
			/// \param registry Registry component belongs to.
			/// \param entity Entity component belongs to.
			///
			void destroy_rigidbody(entt::registry& registry, entt::entity entity);

		public:
			///
			/// The main entt entity registry.
			///
			entt::registry m_registry;

			///
			/// Box2D physics world.
			///
			std::unique_ptr<b2World> m_b2world;

			///
			/// Pointer to the scenes runtime layer.
			///
			scene::RuntimeLayer* m_runtime_layer;

		private:
			///
			/// Stores systems.
			///
			SystemContainer m_systems;

			///
			/// Used to allow for component creation without having to know the compile time type.
			///
			ComponentFactory m_component_factory;

			///
			/// Stores validation configurations.
			///
			robin_hood::unordered_flat_map<std::type_index, std::function<bool(const entt::entity)>> m_validations;

			///
			/// Validations to run upon request.
			///
			std::vector<std::type_index> m_validations_to_run;

			///
			/// Rendersystem index.
			///
			int m_rendersystem_index;

			///
			/// List of rigid bodies that need to be constructed.
			///
			std::vector<std::pair<components::RigidBody*, components::Transform*>> m_bodies_to_construct;

			///
			/// Pointer to scene this world belongs to.
			///
			scene::Scene* m_scene;
		};

		template<meta::valid_component ToValidate, meta::valid_component... Dependencies>
		inline void World::register_dependencies()
		{
			const auto index = std::type_index(typeid(ToValidate));
			if (!m_validations.contains(index))
			{
				m_validations[index] = [this](const entt::entity entity) -> bool {
					const ToValidate* component = m_registry.try_get<ToValidate>(entity);
					if (component)
					{
						return m_registry.all_of<Dependencies...>(entity);
					}
					else
					{
						// If we dont have component needing to be validated, then entity is valid.
						return true;
					}
				};

				m_validations_to_run.push_back(index);
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to register duplicate validation of a component dependency.");
			}
		}

		template<meta::valid_component Component>
		inline void World::register_component(const std::string& name)
		{
			if (!m_component_factory.contains(name))
			{
				m_component_factory.emplace(name, [this](const entt::entity entity, const nlohmann::json& json) {
					this->m_registry.emplace<Component>(entity, json);
				});
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to register duplicate component of type '{0}'.", name);
			}
		}

		template<meta::is_system System, typename... Args>
		inline std::weak_ptr<System> World::create_system(Args&&... args)
		{
			auto ptr = std::make_shared<System>(std::forward<Args>(args)...);
			m_systems.push_back(std::static_pointer_cast<systems::System>(ptr));

			if constexpr (std::is_same<System, systems::RenderSystem>::value)
			{
				m_rendersystem_index = static_cast<int>(m_systems.size()) - 1;
			}

			return ptr;
		}
	} // namespace core
} // namespace galaxy

#endif