///
/// Scene2D.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SCENES_SCENE2D_HPP_
#define GALAXY_SCENES_SCENE2D_HPP_

#include "galaxy/map/TiledWorld.hpp"
#include "galaxy/scenes/Scene.hpp"

namespace galaxy
{
	namespace scene
	{
		///
		/// 2D scene implementation.
		///
		class Scene2D final : public Scene
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
			void on_push() override;

			///
			/// On pop of Scene2D off stack.
			///
			void on_pop() override;

			///
			/// Process events.
			///
			void events() override;

			///
			/// Process update (fixed timestep) logic.
			///
			/// \param dt Delta-Time from fixed timestep gameloop.
			///
			void update(const double dt) override;

			///
			/// Code to be called before rendering. Outside of any glBegin, window.begin(), etc...
			///
			void pre_render() override;

			///
			/// Render to screen.
			///
			void render() override;

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
			Scene2D() = delete;

		public:
			///
			/// Camera.
			///
			graphics::Camera2D m_camera;

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
	} // namespace scene
} // namespace galaxy

#endif