///
/// Layer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATE_LAYER_HPP_
#define GALAXY_STATE_LAYER_HPP_

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/Camera.hpp"

namespace galaxy
{
	namespace state
	{
		///
		/// Rendering/event layer within a scene.
		///
		class Layer final : public fs::Serializable
		{
		public:
			///
			/// Argument Constructor.
			///
			/// \param name Name of the Layer.
			///
			Layer(std::string_view name);

			///
			/// Destructor.
			///
			virtual ~Layer() noexcept;

			///
			/// On push of Layer to stack.
			///
			void on_push();

			///
			/// On pop of Layer off stack.
			///
			void on_pop();

			///
			/// Process events.
			///
			void events();

			///
			/// Process update (fixed timestep) logic.
			///
			void update();

			///
			/// Code to be called before rendering. Outside of any glBegin, window.begin(), etc...
			///
			void pre_render();

			///
			/// Render to screen.
			///
			void render();

			///
			/// Set layer name.
			///
			/// \param name String name for debug purposes.
			///
			void set_name(std::string_view name) noexcept;

			///
			/// Get layer name.
			///
			/// \return Const string reference.
			///
			[[nodiscard]] const std::string& get_name() const noexcept;

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
			/// Constructor.
			///
			Layer() = delete;

		private:
			///
			/// Layer name.
			///
			std::string m_name;

			///
			/// Camera.
			///
			graphics::Camera m_camera;
		};
	} // namespace state
} // namespace galaxy

#endif

/*
#include "galaxy/core/World.hpp"
#include "galaxy/events/Dispatcher.hpp"
#include "galaxy/graphics/Camera2D.hpp"
#include "galaxy/map/TiledWorld.hpp"

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

			///
			/// RML context.
			///
			Rml::Context* m_rml;
*/