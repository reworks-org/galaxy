///
/// RuntimeLayer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATE_LAYER_RUNTIMELAYER_HPP_
#define GALAXY_STATE_LAYER_RUNTIMELAYER_HPP_

#include <glm/vec2.hpp>
#include <tileson.hpp>

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
			/// Get layer type.
			///
			/// \return String.
			///
			[[nodiscard]] const std::string& get_type() const noexcept override;

			///
			/// Load map into scene.
			///
			/// \param key Name of the map to load.
			///
			void load_map(const std::string& key);

			///
			/// Enable the currently active map's entities.
			///
			void enable_map() noexcept;

			///
			/// Disable the currently active map's entities.
			///
			void disable_map() noexcept;

			///
			/// Fetch the key of the loaded map.
			///
			/// \return Const string reference.
			///
			[[nodiscard]] const std::string& get_loaded_map() const noexcept;

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

			///
			/// Recursive function to process tiled map layers.
			///
			/// \param layer Layer to process.
			/// \param level Needs to be incremented for each call.
			///
			void process_layer(tson::Layer& layer, int& level);

			///
			/// Recursive function to process tile layer.
			///
			/// \param layer Layer to process.
			/// \param level Needs to be incremented for each call.
			///
			void process_tile_layer(tson::Layer& layer, int& level);

			///
			/// Recursive function to process object layer.
			///
			/// \param layer Layer to process.
			/// \param level Needs to be incremented for each call.
			///
			void process_object_layer(tson::Layer& layer, int& level);

			///
			/// Recursive function to process image layer.
			///
			/// \param layer Layer to process.
			/// \param level Needs to be incremented for each call.
			///
			void process_image_layer(tson::Layer& layer, int& level);

			///
			/// Fetch tile offset in tileset.
			///
			/// \param tile_id Tile GID to use.
			/// \param map Pointer to tson map.
			/// \param tileset Pointer to tson tileset.
			///
			/// \return Offset as a vec2 integer.
			///
			[[nodiscard]] glm::ivec2 get_tile_offset(const int tile_id, tson::Map* map, tson::Tileset* tileset) noexcept;

		private:
			///
			/// List of entities for currently active map.
			///
			std::vector<entt::entity> m_map_entities;

			///
			/// Currently loaded map.
			///
			std::string m_map;
		};
	} // namespace state
} // namespace galaxy

#endif