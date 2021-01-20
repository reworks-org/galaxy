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

#include "galaxy/map/tile/Tileset.hpp"

namespace galaxy
{
	namespace map
	{
		///
		/// Represents the entire Tiled Map.
		///
		class Map final
		{
		public:
			///
			/// Default constructor.
			///
			Map();

			///
			/// Destructor.
			///
			~Map();

			///
			/// Load a json formatted Tiled map.
			///
			/// \param map File Path to the map to load from disk.
			///
			/// \return True if successful.
			///
			[[maybe_unused]] bool load(std::string_view map);

			///
			/// Load a json file from memory.
			///
			/// \param buffer Pointer to buffer. IS NOT FREED, YOU MUST FREE AFTER CALLING THIS FUNCTION.
			/// \param size Size of the buffer.
			///
			/// \return True if successful.
			///
			[[maybe_unused]] bool load(std::span<char> buffer);

			///
			/// Load from raw json.
			///
			/// \param json JSON object.
			///
			/// \return True if successful.
			///
			[[maybe_unused]] bool move(nlohmann::json&& json);

			///
			/// Parses json structure to member values, etc.
			///
			/// \return True if successful.
			///
			[[maybe_unused]] bool parse();

			///
			/// Get background colour of map.
			///
			/// \return String in format RRGGBB or AARRGGBB.
			///
			[[nodiscard]] std::string get_bg_colour() const;

			///
			/// Get number of tile rows.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int get_height() const;

			///
			/// \brief Get length of the side of a hex tile.
			///
			/// Only hexagonal maps.
			///
			/// \return Const int. In pixels.
			///
			[[nodiscard]] const int get_hex_side_length() const;

			///
			/// Does the map have infinite dimensions.
			///
			/// \return True if is infinite.
			///
			[[nodiscard]] const bool is_infinite() const;

			///
			/// Get map layers.
			///
			/// \return Std::vector of unique_ptrs containing polymorphic layers.
			///
			[[nodiscard]] const auto& get_layers() const;

			///
			/// Returns the next free ID for the creation of a layer.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int get_next_layer_id() const;

			///
			/// Returns the next free ID for the creation of an object.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int get_next_object_id() const;

			///
			/// Map viewpoint orientation.
			///
			/// \return String, format: orthogonal, isometric, staggered or hexagonal.
			///
			[[nodiscard]] std::string get_orientation() const;

			///
			/// Retrieve property.
			/// You will need to provide the type when retrieving.
			///
			/// \param name Name of the property to retrieve.
			///
			/// \return Property cast as type. const.
			///
			template<tiled_property Type>
			[[nodiscard]] const Type get_property(std::string_view name);

			///
			/// \brief Defines order to render tiles in.
			///
			/// Orthogonal maps only.
			///
			/// \return String, format: right-down, right-up, left-down or left-up.
			///
			[[nodiscard]] std::string get_render_order() const;

			///
			/// Get stagger axis.
			///
			/// \return String, format: x or y.
			///
			[[nodiscard]] std::string get_stagger_axis() const;

			///
			/// Get stagger index.
			///
			/// \return String, format: odd or even.
			///
			[[nodiscard]] std::string get_stagger_index() const;

			///
			/// Get tiled version used to save the file.
			///
			/// \return Formatted const string.
			///
			[[nodiscard]] std::string get_tiled_version() const;

			///
			/// Get the map grid height.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int get_tile_height() const;

			///
			/// Get the array of tilesets.
			///
			/// \return Std::vector of tilesets.
			///
			[[nodiscard]] const auto& get_tile_sets() const;

			///
			/// Get the map grid width.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int get_tile_width() const;

			///
			/// Get type of tiled file.
			///
			/// \return String, format: map.
			///
			[[nodiscard]] std::string get_type() const;

			///
			/// Gets the number of tile columns.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int get_width() const;

			///
			/// Get layer compression level.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int get_compression_level() const;

			///
			/// Get raw parsed JSON.
			///
			/// \return JSON object.
			///
			[[nodiscard]] nlohmann::json& raw_json();

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
			/// Hex-formatted colour (#RRGGBB or #AARRGGBB) (optional).
			///
			std::string m_bg_colour;

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
			/// Array of Layers.
			///
			std::vector<std::unique_ptr<Layer>> m_layers;

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
			/// Map of Properties.
			///
			robin_hood::unordered_map<std::string, Property> m_properties;

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
			/// Array of Tilesets.
			///
			std::vector<Tileset> m_tile_sets;

			///
			/// Map grid width.
			///
			int m_tile_width;

			///
			/// Type of tiled file.
			///
			std::string m_type;

			///
			/// Number of tile columns.
			///
			int m_width;

			///
			/// The compression level to use for tile layer data.
			///
			int m_compression_level;
		};

		template<tiled_property Type>
		inline const Type Map::get_property(std::string_view name)
		{
			const auto str = static_cast<std::string>(name);
			return m_properties[str].get<Type>();
		}
	} // namespace map
} // namespace galaxy

#endif