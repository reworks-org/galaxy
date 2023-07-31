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
#include <entt/entt.hpp>
#include <robin_hood.h>

#include "galaxy/fs/Serializable.hpp"
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
			using SystemContainer = meta::vector<std::shared_ptr<systems::System>>;
			using B2BodyFactory   = meta::vector<std::pair<components::RigidBody*, components::Transform*>>;

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
			/// Create an entity from a prefab.
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
			/// Validate an entity to make sure all components have met their requirements as defined by register_dependencies().
			///
			/// \param entity Entity to validate.
			///
			/// \return True if entity is valid.
			///
			[[nodiscard]] bool is_valid(const entt::entity entity);

			///
			/// \brief Get b2World as a raw pointer.
			///
			/// Prefer accessing the unique ptr.
			///
			/// \return Raw pointer. Do NOT delete.
			///
			[[nodiscard]] b2World* b2world();

			///
			/// Get scene this world belongs to.
			///
			/// \return Raw pointer. Do NOT delete.
			///
			[[nodiscard]] const scene::Scene* const scene() const;

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

			///
			/// Function that integrates nuklear init with entt on construct event.
			///
			/// \param registry Registry component belongs to.
			/// \param entity Entity component belongs to.
			///
			void construct_nui(entt::registry& registry, entt::entity entity);

			///
			/// Function that integrates nuklear destroy with entt on destruction event.
			///
			/// \param registry Registry component belongs to.
			/// \param entity Entity component belongs to.
			///
			void destruct_nui(entt::registry& registry, entt::entity entity);

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
			/// Box2D world velocity iterations.
			///
			int m_velocity_iterations;

			///
			/// Box2d world position iterations.
			///
			int m_position_iterations;

			///
			/// Scale for converting from box2d units to opengl pixels.
			///
			float m_pixels_per_meter;

		private:
			///
			/// Stores systems.
			///
			SystemContainer m_systems;

			///
			/// Rendersystem index.
			///
			int m_rendersystem_index;

			///
			/// List of rigid bodies that need to be constructed.
			///
			B2BodyFactory m_bodies_to_construct;

			///
			/// Pointer to scene this world belongs to.
			///
			scene::Scene* m_scene;
		};

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