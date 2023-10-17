///
/// Scene.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATE_SCENE_HPP_
#define GALAXY_STATE_SCENE_HPP_

#include <entt/signal/dispatcher.hpp>

#include "galaxy/core/World.hpp"
#include "galaxy/graphics/Camera.hpp"

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

		  public:
			///
			/// Camera.
			///
			graphics::Camera m_camera;

			///
			/// Game dispatcher.
			///
			entt::dispatcher m_dispatcher;

			///
			/// Entity data and processing systems.
			///
			core::World m_world;

			///
			/// Scene name for debug purposes.
			///
			std::string m_name;
		};
	} // namespace scene
} // namespace galaxy

#endif
