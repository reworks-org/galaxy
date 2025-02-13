///
/// Scene.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATE_SCENE_HPP_
#define GALAXY_STATE_SCENE_HPP_

#include <entt/signal/dispatcher.hpp>

#include "galaxy/core/Registry.hpp"
#include "galaxy/graphics/Camera.hpp"
#include "galaxy/map/World.hpp"
#include "galaxy/systems/RenderSystem.hpp"

namespace galaxy
{
	namespace scene
	{
		///
		/// Represents a scene in a game. Like the menu, game, etc.
		///	Does not share resources.
		///
		class Scene : public fs::Serializable
		{
		public:
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
			/// Loads an LDTK world for this scene.
			///
			/// \param file .ldtk project file to load.
			///
			/// \return True if loaded successfully.
			///
			[[nodiscard]]
			bool load_world(const std::string& file);

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to write out.
			///
			[[nodiscard]]
			nlohmann::json serialize() override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			void deserialize(const nlohmann::json& json) override;

		private:
			///
			/// Constructor.
			///
			Scene() = delete;

		public:
			///
			/// Scene name for debug purposes.
			///
			std::string m_name;

			///
			/// Camera.
			///
			graphics::Camera m_camera;

			///
			/// Scene event handler.
			///
			entt::dispatcher m_dispatcher;

			///
			/// Entity data.
			///
			core::Registry m_registry;

			///
			/// Box2D physics world.
			///
			b2World m_b2world;

			///
			/// LDTK world.
			///
			map::World m_world;

			///
			/// Box2D world velocity iterations.
			///
			int m_velocity_iterations;

			///
			/// Box2d world position iterations.
			///
			int m_position_iterations;
		};
	} // namespace scene
} // namespace galaxy

#endif
