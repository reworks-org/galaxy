///
/// Scene.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATE_SCENE_HPP_
#define GALAXY_STATE_SCENE_HPP_

#include <entt/signal/dispatcher.hpp>
#include <RmlUi/Core/Context.h>

#include "galaxy/core/World.hpp"
#include "galaxy/core/TiledMap.hpp"
#include "galaxy/graphics/Camera.hpp"
#include "galaxy/graphics/Lighting.hpp"

namespace galaxy
{
	namespace core
	{
		class Window;
	} // namespace core

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
			/// Event processing method for window size change.
			///
			/// \param e Takes in a window resized event.
			///
			void on_window_resized(const events::WindowResized& e);

			///
			/// Process events and updates.
			///
			virtual void update();

			///
			/// Render scene.
			///
			virtual void render();

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
			/// UI rendering context.
			///
			Rml::Context* m_context;

			///
			/// Manages the maps for this layer.
			///
			core::TiledMap m_map;

			///
			/// Scene name for debug purposes.
			///
			std::string m_name;

			///
			/// Scene lighting.
			///
			graphics::Lighting m_lighting;

		protected:
			///
			/// Pointer to window.
			///
			core::Window* m_window;
		};
	} // namespace scene
} // namespace galaxy

#endif