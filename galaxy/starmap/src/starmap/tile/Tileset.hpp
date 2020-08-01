///
/// Tileset.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_TILESET_HPP_
#define STARMAP_TILESET_HPP_

#include "starmap/tile/Tile.hpp"
#include "starmap/types/Grid.hpp"
#include "starmap/types/Terrain.hpp"
#include "starmap/tile/TileOffset.hpp"
#include "starmap/wangtile/WangSet.hpp"

///
/// Core namespace.
///
namespace starmap
{
	///
	/// A tileset is a collection of tiles. Two types:
	/// Based on Tileset Image:
	///		This tileset defines a fixed size for all tilesand the image from which these tiles are supposed to be cut.In addition it supports a margin around the tilesand a spacing between the tiles, which allows for using tileset images that either happen to have space between or around their tiles or those that have extruded the border pixels of each tile to avoid color bleeding.
	/// Collection of Images:
	///		In this type of tileset each tile refers to its own image file.It is useful when the tiles aren’t the same size, or when the packing of tiles into a texture is done later on.
	///
	class Tileset final
	{
	public:
		///
		/// Default constructor.
		///
		Tileset() noexcept;

		///
		/// \brief Parse constructor.
		///
		/// Can throw exceptions.
		///
		/// \param json JSON structure/array containing Tileset.
		///
		explicit Tileset(const nlohmann::json& json);

		///
		/// Destructor.
		///
		~Tileset() noexcept;

		///
		/// \brief Parses json structure to member values, etc.
		///
		/// You can surround this with a try-catch block since it does throw exceptions.
		///
		/// \param json JSON structure/array containing Tileset.
		///
		void parse(const nlohmann::json& json);

		///
		/// Get background colour of tileset.
		///
		/// \return String in format RRGGBB or AARRGGBB.
		///
		[[nodiscard]] std::string get_bg_colour() const noexcept;

		///
		/// Get total tile columns in tileset.
		///
		/// \return Const int.
		///
		[[nodiscard]] const int get_columns() const noexcept;

		///
		/// Get grid id of first tile in set.
		///
		/// \return Const int.
		///
		[[nodiscard]] const int get_first_gid() const noexcept;

		///
		/// Get the grid used by the tileset.
		///
		/// \return Returns a std::optional. Make sure you check for std::nullopt if grid is not used!
		///
		[[nodiscard]] const auto& get_grid() const noexcept;

		///
		/// Get image.
		///
		/// \return String in format RRGGBB or AARRGGBB.
		///
		[[nodiscard]] std::string get_image() const noexcept;

		///
		/// Get image height in pixels.
		///
		/// \return Const int.
		///
		[[nodiscard]] const int get_image_height() const noexcept;

		///
		/// Get image width in pixels.
		///
		/// \return Const int.
		///
		[[nodiscard]] const int get_image_width() const noexcept;

		///
		/// Get buffer between image edge and first tile.
		///
		/// \return Const int.
		///
		[[nodiscard]] const int get_margin() const noexcept;

		///
		/// Get tileset name.
		///
		/// \return Name as string.
		///
		[[nodiscard]] std::string get_name() const noexcept;

		///
		/// Alignment to use for tile objects.
		///
		/// \return unspecified (default), topleft, top, topright, left, center, right, bottomleft, bottom or bottomright.
		///
		[[nodiscard]] std::string get_object_alignment() const noexcept;

		///
		/// \brief Retrieve property.
		///
		/// You will need to provide the type when retrieving.
		///
		/// \param name Name of the property to retrieve.
		///
		/// \return Property cast as type.
		///
		template<tiled_property Type>
		[[nodiscard]] const Type get_property(std::string_view name) noexcept;

		///
		/// Get array of terrains.
		///
		/// \return Std::vector containing starmap::Terrain.
		///
		[[nodiscard]] const auto& get_terrain() const noexcept;

		///
		/// Get the number of tiles.
		///
		/// \return Const int.
		///
		[[nodiscard]] const int get_tile_count() const noexcept;

		///
		/// Get version of Tiled used to save tileset.
		///
		/// \return Const std::string.
		///
		[[nodiscard]] std::string get_tiled_version() const noexcept;

		///
		/// Get the maximum height of tiles.
		///
		/// \return Const int.
		///
		[[nodiscard]] const int get_tile_height() const noexcept;

		///
		/// Get the tile offset used by the tileset.
		///
		/// \return Returns a std::optional. Make sure you check for std::nullopt if tile offset is not used!
		///
		[[nodiscard]] const auto& get_tile_offset() const noexcept;

		///
		/// Get the tiles in the tileset.
		///
		/// \return Std::vector array.
		///
		[[nodiscard]] const auto& get_tiles() const noexcept;

		///
		/// Get the maximum width of tiles.
		///
		/// \return Const int.
		///
		[[nodiscard]] const int get_tile_width() const noexcept;

		///
		/// Get colour used to mark an area transparent.
		///
		/// \return Const std::string reference. Hex-formatted. Defaults to White (FFFFFF).
		///
		[[nodiscard]] std::string get_transparent_colour() const noexcept;

		///
		/// Get type of tileset.
		///
		/// \return Const std::string reference. Defaults to "tileset".
		///
		[[nodiscard]] std::string get_type() const noexcept;

		///
		/// Get wang sets.
		///
		/// \return Std::vector array.
		///
		[[nodiscard]] const auto& get_wang_sets() const noexcept;

	private:
		///
		/// Hex-formatted color (#RRGGBB or #AARRGGBB) (optional).
		///
		std::string m_bg_colour;

		///
		/// The number of tile columns in the tileset.
		///
		int m_columns;

		///
		/// GID corresponding to the first tile in the set.
		///
		int m_first_gid;

		///
		/// Determines how tile overlays for terrain and collision information are rendered.
		///
		std::optional<starmap::Grid> m_grid;

		///
		/// Image used for tiles in this set.
		///
		std::string m_image;

		///
		/// Height of source image in pixels.
		///
		int m_image_height;

		///
		/// Width of source image in pixels
		///
		int m_image_width;

		///
		/// Buffer between image edgeand first tile (pixels).
		///
		int m_margin;

		///
		/// Name given to this tileset.
		///
		std::string m_name;

		///
		/// Alignment to use for tile objects.
		///
		std::string m_object_alignment;

		///
		/// Map of Properties.
		///
		std::unordered_map<std::string, starmap::Property> m_properties;

		///
		/// The external file that contains this tilesets data.
		///
		std::string m_source;

		///
		/// Spacing between adjacent tiles in image (pixels).
		///
		int m_spacing;

		///
		/// Array of Terrains (optional).
		///
		std::vector<starmap::Terrain> m_terrain;

		///
		/// The number of tiles in this tileset.
		///
		int m_tile_count;

		///
		/// The Tiled version used to save the file.
		///
		std::string m_tiled_version;

		///
		/// Maximum height of tiles in this set.
		///
		int m_tile_height;

		///
		/// Offset in pixels, to be applied when drawing a tile from the related tileset
		///
		std::optional<starmap::TileOffset> m_tile_offset;

		///
		/// Array of tiles.
		///
		std::vector<starmap::Tile> m_tiles;

		///
		/// Maximum width of tiles in this set.
		///
		int m_tile_width;

		///
		/// Hex - formatted color(#RRGGBB) (optional)
		///
		std::string m_transparent_colour;

		///
		/// Tileset(for tileset files, since 1.0).
		///
		std::string m_type;

		///
		/// Array of Wang sets.
		///
		std::vector<starmap::WangSet> m_wang_sets;
	};

	template<tiled_property Type>
	inline const Type Tileset::get_property(std::string_view name) noexcept
	{
		const auto str = static_cast<std::string>(name);
		return m_properties[str].get<Type>();
	}
} // namespace starmap

#endif