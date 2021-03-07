///
/// Map.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MAP_MAP_HPP_
#define GALAXY_MAP_MAP_HPP_

#include <cute_tiled.h>

#include "galaxy/graphics/Camera.hpp"
#include "galaxy/graphics/SpriteBatch.hpp"

namespace galaxy
{
	namespace map
	{
		///
		/// Shorthand.
		///
		using TileWithSet = std::tuple<cute_tiled_tile_descriptor_t*, cute_tiled_tileset_t*>;

		///
		/// Shorthand.
		///
		using BatchData = std::pair<std::unique_ptr<components::BatchSprite>, std::unique_ptr<components::Transform>>;

		///
		/// Represents the entire Tiled Map.
		///
		class Map final
		{
		public:
			struct RenderData final
			{
				std::unique_ptr<graphics::SpriteBatch> m_batch;
				std::vector<std::unique_ptr<BatchData>> m_batch_data;
			};

		public:
			///
			/// Constructor.
			///
			Map() noexcept;

			///
			/// Argument constructor.
			///
			/// \param map Path to the map to load from disk.
			///
			Map(std::string_view map) noexcept;

			///
			/// Move constructor.
			///
			Map(Map&&) noexcept;

			///
			/// Move assignment operator.
			///
			Map& operator=(Map&&) noexcept;

			///
			/// Destructor.
			///
			~Map() noexcept;

			///
			/// \brief Load a json formatted Tiled map.
			///
			/// Does not support external tilesets. CSV format only.
			///
			/// \param map Path to the map to load from disk.
			///
			/// \return True if successful.
			///
			[[maybe_unused]] const bool load(std::string_view map);

			///
			/// Updates spritebatch data.
			///
			void update_batches();

			///
			/// Render map.
			///
			/// \param camera Camera view to draw map to.
			///
			void render(graphics::Camera& camera);

		private:
			///
			/// Parses all tilesets into texture atlas.
			///
			void parse_tilesets();

			///
			/// \brief Parse layers.
			///
			/// Is a seperate function to process groups.
			///
			/// \param layer Tilemap layer.
			/// \param level Z Level of layer.
			///
			/// \return Updated level count.
			///
			[[nodiscard]] const int parse_layers(cute_tiled_layer_t* layer, int level);

			///
			/// Parse image layer.
			///
			/// \param layer Image layer.
			/// \param level Z Level of layer.
			///
			void parse_image_layer(cute_tiled_layer_t* layer, int level);

			///
			/// Parse object layer.
			///
			/// \param layer Object layer.
			/// \param level Z Level of layer.
			///
			void parse_object_layer(cute_tiled_layer_t* layer, int level);

			///
			/// Parse tile layer.
			///
			/// \param layer Tile layer.
			/// \param level Z Level of layer.
			///
			void parse_tile_layer(cute_tiled_layer_t* layer, int level);

			///
			/// Gets tile and tileset.
			///
			/// \return Pointer to Tile and Tileset.
			///
			[[nodiscard]] TileWithSet get_tile_with_set(const int gid);

		private:
			///
			/// Map data.
			///
			cute_tiled_map_t* m_map;

			///
			/// Render data.
			///
			std::vector<std::unique_ptr<RenderData>> m_data;
		};
	} // namespace map
} // namespace galaxy

#endif