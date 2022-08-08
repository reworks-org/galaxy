///
/// Scene.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATE_SCENE_HPP_
#define GALAXY_STATE_SCENE_HPP_

#include "galaxy/state/Layers.hpp"

namespace galaxy
{
	namespace state
	{
		///
		/// Represents a scene in a game. Like the menu, game, etc.
		///	Does not share resources.
		///
		class Scene final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Scene() noexcept;

			///
			/// Destructor.
			///
			virtual ~Scene() noexcept;

			///
			/// \brief Load data required by this scene.
			///
			/// Displays a loading screen overriding whatever is currently rendering.
			///
			void load();

			///
			/// Unload scene data.
			///
			void unload();

			///
			/// Process scene events.
			///
			void events();

			///
			/// Process updates.
			///
			void update();

			///
			/// Render scene.
			///
			void render();

			///
			/// Set Scene name.
			///
			/// \param name String name for debug purposes.
			///
			void set_name(const std::string& name) noexcept;

			///
			/// Get Scene name.
			///
			/// \return Const std::string reference.
			///
			[[nodiscard]] const std::string& get_name() const noexcept;

			///
			/// Get layer stack.
			///
			/// \return Reference to layers stack.
			///
			[[nodiscard]] Layers& get_layers() noexcept;

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
			/// Scene name for debug purposes.
			///
			std::string m_name;

			///
			/// Controls current application layer.
			///
			Layers m_layer_stack;
		};
	} // namespace state
} // namespace galaxy

#endif

/*
#include "galaxy/core/World.hpp"
#include "galaxy/events/Dispatcher.hpp"
#include "galaxy/graphics/Camera2D.hpp"
#include "galaxy/map/TiledWorld.hpp"

		///
		/// Rendering/event Scene within a scene.
		/// Scenes -> Game, Interiors, Pause, etc.
		/// Share Resources, in a stack with only whats on top being processed.
		///

namespace Rml
{
	class Context;
} // namespace Rml
	namespace systems
	{
		class ActionSystem;
		class RenderSystem2D;
	} // namespace systems


		///
			/// Set Scene resources.
			///
			/// \param resources Scene resources.
			///
			void set_resources(resource::Resources* resources) noexcept;

	///
			/// Load an RML document.
			///
			/// \param document File, including path.
			///
			void load_rml_doc(std::string_view document);

			///
			/// Load a tiled world.
			///
			/// \param path Path to the tiled world.
			///
			void create_maps(std::string_view path);

			///
			/// Set the active map.
			///
			/// \param name Name of the map file to set as active.
			///
			void set_active_map(std::string_view name);

			///
			/// Get a map.
			///
			/// \param name Name of the map file to get.
			///
			/// \return Pointer to the map.
			///
			[[nodiscard]] map::Map* get_map(std::string_view name);

			///
			/// Get active map.
			///
			/// \return Pointer to the map.
			///
			[[nodiscard]] map::Map* get_active_map();




			///
			/// Entity/System manager.
			///
			core::World m_world;


		private:
			///
			/// Tiled map world.
			///
			map::TiledWorld m_maps;

			///
			/// Currently active map.
			///
			std::string m_active_map;

			///
			/// Current map path.
			///
			std::string m_maps_path;

			///
			/// Pointer to rendering system.
			///
			systems::RenderSystem2D* m_rendersystem;

			///
			/// RML context.
			///
			Rml::Context* m_rml;///
			/// Pointer to scene resources.
			///
			resource::Resources* m_resources;

			///
			/// Pointer to window.
			///
			core::Window* m_window;

			///
			/// Camera.
			///
			graphics::Camera m_camera;


*/