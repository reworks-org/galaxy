///
/// Scene2D.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_SCENE2D_HPP_
#define GALAXY_CORE_SCENE2D_HPP_

#include "galaxy/core/World.hpp"
#include "galaxy/events/Dispatcher.hpp"
#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/Camera2D.hpp"
#include "galaxy/map/TiledWorld.hpp"

namespace galaxy
{
	namespace systems
	{
		class RenderSystem2D;
	} // namespace systems

	namespace core
	{
		///
		/// 2D scene implementation.
		///
		class Scene2D final : public fs::Serializable
		{
		public:
			///
			/// Argument Constructor.
			///
			/// \param name Name of the Scene2D.
			///
			Scene2D(std::string_view name) noexcept;

			///
			/// Destructor.
			///
			virtual ~Scene2D() noexcept;

			///
			/// On push of Scene2D to stack.
			///
			void on_push();

			///
			/// On pop of Scene2D off stack.
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
			/// Constructor.
			///
			Scene2D() = delete;

		public:
			///
			/// Scene name.
			///
			std::string m_name;

			///
			/// Camera.
			///
			graphics::Camera2D m_camera;

			///
			/// Entity/System manager.
			///
			core::World m_world;

			///
			/// Event dispatcher.
			///
			events::Dispatcher m_dispatcher;

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
		};
	} // namespace core
} // namespace galaxy

#endif