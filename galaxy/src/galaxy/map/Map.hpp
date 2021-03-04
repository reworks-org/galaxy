///
/// Map.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MAP_MAP_HPP_
#define GALAXY_MAP_MAP_HPP_

#include <span>

#include <nlohmann/json.hpp>

#include "galaxy/map/layer/TileLayer.hpp"
#include "galaxy/map/layer/ObjectLayer.hpp"
#include "galaxy/map/layer/ImageLayer.hpp"
#include "galaxy/map/tile/Tileset.hpp"

namespace galaxy
{
	namespace core
	{
		class World;
	} // namespace core

	namespace map
	{
		///
		/// Represents the entire Tiled Map.
		///
		class Map final
		{
		public:
			///
			/// Constructor.
			///
			Map() noexcept;

			///
			/// Destructor.
			///
			~Map() noexcept;

			///
			/// \brief Load a json formatted Tiled map.
			///
			/// Only supports base64 uncompressed or base64 zlib/gzip. Does not support external tilesets.
			///
			/// \param map File Path to the map to load from disk.
			///
			/// \return True if successful.
			///
			[[maybe_unused]] const bool load(std::string_view map);

			///
			/// \brief Load a json file from memory.
			///
			/// Only supports base64 uncompressed or base64 zlib/gzip. Does not support external tilesets.
			///
			/// \param buffer Pointer to buffer. IS NOT FREED, YOU MUST FREE AFTER CALLING THIS FUNCTION.
			///
			/// \return True if successful.
			///
			[[maybe_unused]] const bool load_mem(std::span<char> buffer);

			///
			/// Parses json structure to member values, etc.
			///
			/// \return True if successful.
			///
			[[maybe_unused]] const bool parse();

			///
			/// Generate entities for image layers.
			///
			/// \param world World to create entities in.
			///
			void generate_image_entities(core::World& world);

			///
			/// Generate entities from objects.
			///
			/// \param world World to create entities in.
			///
			void generate_object_entities(core::World& world);

			///
			/// Generate map entities for tiles.
			///
			/// \param world World to create entities in.
			///
			void generate_map_entities(core::World& world);

			///
			/// Get background colour of map.
			///
			/// \return Const reference to a graphics::Colour object.
			///
			[[nodiscard]] const graphics::Colour& get_bg_colour() const noexcept;

			///
			/// Get layer compression level.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int get_compression_level() const noexcept;

			///
			/// Get number of tile rows.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int get_height() const noexcept;

			///
			/// \brief Get length of the side of a hex tile.
			///
			/// Only hexagonal maps.
			///
			/// \return Const int. In pixels.
			///
			[[nodiscard]] const int get_hex_side_length() const noexcept;

			///
			/// Does the map have infinite dimensions.
			///
			/// \return True if is infinite.
			///
			[[nodiscard]] const bool is_infinite() const noexcept;

			///
			/// Get tile layers.
			///
			/// \return Const reference to vector.
			///
			[[nodiscard]] const std::vector<TileLayer>& tile_layers() const noexcept;

			///
			/// Get object layers.
			///
			/// \return Const reference to vector.
			///
			[[nodiscard]] const std::vector<ObjectLayer>& object_layers() const noexcept;

			///
			/// Get image layers.
			///
			/// \return Const reference to vector.
			///
			[[nodiscard]] const std::vector<ImageLayer>& image_layers() const noexcept;

			///
			/// Returns the next free ID for the creation of a layer.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int get_next_layer_id() const noexcept;

			///
			/// Returns the next free ID for the creation of an object.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int get_next_object_id() const noexcept;

			///
			/// Map viewpoint orientation.
			///
			/// \return String, format: orthogonal, isometric, staggered or hexagonal.
			///
			[[nodiscard]] const std::string& get_orientation() const noexcept;

			///
			/// Retrieve property.
			/// You will need to provide the type when retrieving.
			///
			/// \param name Name of the property to retrieve.
			///
			/// \return Property cast as type. const.
			///
			template<tiled_property Type>
			[[nodiscard]] const Type& get_property(std::string_view name);

			///
			/// \brief Defines order to render tiles in.
			///
			/// Orthogonal maps only.
			///
			/// \return String, format: right-down, right-up, left-down or left-up.
			///
			[[nodiscard]] const std::string& get_render_order() const noexcept;

			///
			/// Get stagger axis.
			///
			/// \return String, format: x or y.
			///
			[[nodiscard]] const std::string& get_stagger_axis() const noexcept;

			///
			/// Get stagger index.
			///
			/// \return String, format: odd or even.
			///
			[[nodiscard]] const std::string& get_stagger_index() const noexcept;

			///
			/// Get tiled version used to save the file.
			///
			/// \return Formatted const string.
			///
			[[nodiscard]] const std::string& get_tiled_version() const noexcept;

			///
			/// Get the map grid height.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int get_tile_height() const noexcept;

			///
			/// Get the array of tilesets.
			///
			/// \return Tileset hashmap.
			///
			[[nodiscard]] const robin_hood::unordered_flat_map<std::string, Tileset>& get_tile_sets() const noexcept;

			///
			/// Get the map grid width.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int get_tile_width() const noexcept;

			///
			/// Get type of tiled file.
			///
			/// \return String, format: map.
			///
			[[nodiscard]] const std::string& get_type() const noexcept;

			///
			/// Get JSON format version.
			///
			/// \return Const double.
			///
			[[nodiscard]] const double get_version() const noexcept;

			///
			/// Gets the number of tile columns.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int get_width() const noexcept;

			///
			/// Get raw parsed JSON.
			///
			/// \return JSON object.
			///
			[[nodiscard]] const nlohmann::json& get_raw() const noexcept;

		private:
			///
			/// \brief Parse layers.
			///
			/// Is a seperate function to process groups.
			///
			/// \param json Tilemap layers json.
			/// \param level Z Level of layer.
			///
			/// \return Updated level count.
			///
			const int parse_layers(const nlohmann::json& json, int level);

		private:
			///
			/// Flag for seeing if json is loaded.
			///
			bool m_loaded;

			///
			/// Maser root node of map file.
			///
			nlohmann::json m_root;

			///
			/// Map background colour.
			///
			graphics::Colour m_bg_colour;

			///
			/// The compression level to use for tile layer data.
			///
			int m_compression_level;

			///
			/// Number of tile rows.
			///
			int m_height;

			///
			/// Length of the side of a hex tile in pixels (hexagonal maps only).
			///
			int m_hex_side_length;

			///
			/// Whether the map has infinite dimensions.
			///
			bool m_infinite;

			///
			/// Tile layers.
			///
			std::vector<TileLayer> m_tile_layers;

			///
			/// Object layers.
			///
			std::vector<ObjectLayer> m_object_layers;

			///
			/// Image layers.
			///
			std::vector<ImageLayer> m_image_layers;

			///
			/// Auto-increments for each layer.
			///
			int m_next_layer_id;

			///
			/// Auto-increments for each placed object.
			///
			int m_next_object_id;

			///
			/// Map viewpoint orientation.
			/// orthogonal, isometric, staggered or hexagonal.
			///
			std::string m_orientation;

			///
			/// Fast hashmap of properties.
			///
			robin_hood::unordered_flat_map<std::string, Property> m_properties;

			///
			/// Right-down (the default), right-up, left-down or left-up (orthogonal maps only).
			///
			std::string m_render_order;

			///
			/// X or y (staggered / hexagonal maps only).
			///
			std::string m_stagger_axis;

			///
			/// Odd or even (staggered / hexagonal maps only);
			///
			std::string m_stagger_index;

			///
			/// The Tiled version used to save the file.
			///
			std::string m_tiled_version;

			///
			/// Map grid height.
			///
			int m_tile_height;

			///
			/// Hashmap of Tilesets.
			///
			robin_hood::unordered_flat_map<std::string, Tileset> m_tile_sets;

			///
			/// Map grid width.
			///
			int m_tile_width;

			///
			/// Type of tiled file.
			///
			std::string m_type;

			///
			/// The JSON format version.
			///
			double m_version;

			///
			/// Number of tile columns.
			///
			int m_width;
		};

		template<tiled_property Type>
		inline const Type& Map::get_property(std::string_view name)
		{
			const auto str = static_cast<std::string>(name);
			return m_properties[str].get<Type>();
		}
	} // namespace map
} // namespace galaxy

#endif