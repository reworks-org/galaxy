///
/// MapLayer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATE_LAYER_MAPLAYER_HPP_
#define GALAXY_STATE_LAYER_MAPLAYER_HPP_

#include <nlohmann/json_fwd.hpp>
#include <tileson/tileson.h>

#include "galaxy/state/Layer.hpp"

namespace galaxy
{
	namespace state
	{
		///
		/// Layer specifically for maps.
		///
		class MapLayer final : public Layer
		{
		public:
			///
			/// Argument Constructor.
			///
			/// \param name Name of the Layer.
			/// \param scene Pointer to scene this layer belongs to.
			///
			MapLayer(std::string_view name, Scene* scene) noexcept;

			///
			/// Destructor.
			///
			virtual ~MapLayer() noexcept;

			///
			/// On push of MapLayer to stack.
			///
			void on_push() override;

			///
			/// On pop of MapLayer off stack.
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
			MapLayer() = delete;
		};
	} // namespace state
} // namespace galaxy

#endif

/*
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