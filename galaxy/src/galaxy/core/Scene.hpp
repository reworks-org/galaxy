///
/// Scene.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SCENES_SCENE_HPP_
#define GALAXY_SCENES_SCENE_HPP_

#include "galaxy/core/World.hpp"
#include "galaxy/map/TiledWorld.hpp"
#include "galaxy/ui/GUI.hpp"

namespace galaxy
{
	namespace core
	{
		///
		/// Represents a scene within an instance.
		///
		class Scene final : public fs::Serializable
		{
		public:
			///
			/// Argument Constructor.
			///
			/// \param name Name of the scene.
			///
			Scene(std::string_view name) noexcept;

			///
			/// Destructor.
			///
			~Scene() noexcept;

			///
			/// On push of scene to stack.
			///
			void on_push();

			///
			/// On pop of scene off stack.
			///
			void on_pop();

			///
			/// Process events.
			///
			void events();

			///
			/// Process update (fixed timestep) logic.
			///
			/// \param dt Delta-Time from fixed timestep gameloop.
			///
			void update(const double dt);

			///
			/// Code to be called before rendering. Outside of any glBegin, window.begin(), etc...
			///
			void pre_render();

			///
			/// Render to screen.
			///
			void render();

			///
			/// Load a tiled world.
			///
			/// \param path Path to the tiled world.
			///
			void create_maps(std::string_view path);

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
			/// Set the active map.
			///
			/// \param name Name of the map file to set as active.
			///
			void set_active_map(std::string_view name);

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
			/// Deleted default constructor.
			///
			Scene() = delete;

		public:
			///
			/// Scene name.
			///
			std::string m_name;

			///
			/// Entity/System manager.
			///
			core::World m_world;

			///
			/// Camera.
			///
			graphics::Camera m_camera;

			///
			/// Event dispatcher.
			///
			events::Dispatcher m_dispatcher;

			///
			/// GUI theme.
			///
			ui::Theme m_gui_theme;

			///
			/// GUI for this scene.
			///
			ui::GUI m_gui;

		private:
			///
			/// Tiled map world.
			///
			map::TiledWorld m_maps;

			///
			/// Currently active map.
			///
			std::string m_active_map;
		};
	} // namespace core
} // namespace galaxy

#endif