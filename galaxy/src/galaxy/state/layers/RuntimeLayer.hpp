///
/// RuntimeLayer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATE_LAYER_RUNTIMELAYER_HPP_
#define GALAXY_STATE_LAYER_RUNTIMELAYER_HPP_

#include "galaxy/state/Layer.hpp"

namespace galaxy
{
	namespace state
	{
		///
		/// A layer is usually a layering of events/updates/rendering grouped together that dont interact.
		/// I.e. UI, Debug UI, Game, etc.
		///
		class RuntimeLayer final : public Layer
		{
		public:
			///
			/// Argument Constructor.
			///
			/// \param name Name of the Layer.
			/// \param scene Pointer to scene this layer belongs to.
			///
			RuntimeLayer(std::string_view name, Scene* scene) noexcept;

			///
			/// Destructor.
			///
			virtual ~RuntimeLayer() noexcept;

			///
			/// On push of RuntimeLayer to stack.
			///
			void on_push() override;

			///
			/// On pop of RuntimeLayer off stack.
			///
			void on_pop() override;

			///
			/// Handle events.
			///
			void events() override;

			///
			/// Process events/updates and prepare for rendering.
			///
			void update() override;

			///
			/// Render layer.
			///
			void render() override;

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
			RuntimeLayer() = delete;
		};
	} // namespace state
} // namespace galaxy

#endif

/*

		///
			/// Add a tiled project to this layer.
			///
			/// \param path Path to the project.
			///
			void add_tiled_project(std::string_view path);

			///
			/// Parses the predefined list of tiled projects
			///
			void parse_tiled_projects();

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
*/