///
/// Scene.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATE_SCENE_HPP_
#define GALAXY_STATE_SCENE_HPP_

#include <box2d/b2_world.h>

#include "galaxy/graphics/Camera.hpp"
#include "galaxy/map/Map.hpp"
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
		///
		/// Represents a scene in a game. Like the menu, game, etc.
		///	Does not share resources.
		///
		class Scene : public fs::Serializable
		{
			using B2BodyFactory   = meta::vector<std::pair<components::RigidBody*, components::Transform*>>;
			using SystemContainer = meta::vector<std::shared_ptr<systems::System>>;

		  public:
			///
			/// Constructor.
			///
			Scene();

			///
			/// Name constructor.
			///
			/// \param name Name of the scene for debug / editor purposes.
			///
			Scene(const std::string& name);

			///
			/// Destructor.
			///
			virtual ~Scene();

			///
			/// When scene is loaded and made active.
			///
			virtual void load();

			///
			/// When scene is deactivated / unloaded.
			///
			virtual void unload();

			///
			/// Process events and updates.
			///
			virtual void update();

			///
			/// Render scene.
			///
			virtual void render();

			///
			/// Event processing method for window size change.
			///
			/// \param e Takes in a window resized event.
			///
			void on_window_resized(const events::WindowResized& e);

			///
			/// Only update ui.
			///
			void update_ui();

			///
			/// Only update rendering.
			///
			void update_rendering();

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
			/// Validate an entity to make sure all components have met their requirements as defined by register_dependencies().
			///
			/// \param entity Entity to validate.
			///
			/// \return True if entity is valid.
			///
			[[nodiscard]] bool is_valid(const entt::entity entity);

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
			/// Load maps.
			///
			void load_maps();

			///
			/// Set currently active map.
			///
			/// \param map Name of map to use.
			///
			void set_active_map(const std::string& map);

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to write out.
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
			/// Shared init function for constructors.
			///
			void init();

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
			void destroy_rigidbody(entt::registry& registry, entt::entity entity) const;

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

			///
			/// Function that runs when an entity is enabled.
			///
			/// \param registry Registry component belongs to.
			/// \param entity Entity component belongs to.
			///
			void enable_entity(entt::registry& registry, entt::entity entity);

			///
			/// Function that runs when an entity is disabled.
			///
			/// \param registry Registry component belongs to.
			/// \param entity Entity component belongs to.
			///
			void disable_entity(entt::registry& registry, entt::entity entity);

		  public:
			///
			/// Is this scene enabled.
			///
			bool m_enabled;

			///
			/// Scene name for debug purposes.
			///
			std::string m_name;

			///
			/// Camera.
			///
			graphics::Camera m_camera;

			///
			/// The main entt entity registry.
			///
			entt::registry m_registry;

			///
			/// Stores systems.
			///
			SystemContainer m_systems;

			///
			/// Rendersystem index.
			///
			std::size_t m_rendersystem_index;

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
			/// List of rigid bodies that need to be constructed.
			///
			B2BodyFactory m_bodies_to_construct;

			///
			/// Maps assigned to this scene.
			///
			std::vector<std::string> m_assigned_maps;

			///
			/// Currently active map.
			///
			std::shared_ptr<map::Map> m_map;
		};

		template<meta::is_system System, typename... Args>
		inline std::weak_ptr<System> Scene::create_system(Args&&... args)
		{
			auto ptr = std::make_shared<System>(std::forward<Args>(args)...);
			m_systems.push_back(std::static_pointer_cast<systems::System>(ptr));

			if constexpr (std::is_same<System, systems::RenderSystem>::value)
			{
				m_rendersystem_index = m_systems.size() - 1;
			}

			return ptr;
		}
	} // namespace scene
} // namespace galaxy

#endif
