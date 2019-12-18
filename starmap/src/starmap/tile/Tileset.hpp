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
		/// Copy constructor.
		///
		Tileset(const Tileset&) = default;

		///
		/// Move constructor.
		///
		Tileset(Tileset&&) = default;

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
		/// \return String, format: #RRGGBB or #AARRGGBB.
		///
		const std::string& getBackgroundColour() const noexcept;

		///
		/// Get total tile columns in tileset.
		///
		/// \return Const int.
		///
		const int getColumns() const noexcept;

		///
		/// Get grid id of first tile in set.
		///
		/// \return Const int.
		///
		const int getFirstGID() const noexcept;

		///
		/// Get the grid used by the tileset.
		///
		/// \return Returns a std::optional. Make sure you check for std::nullopt if grid is not used!
		///
		const auto& getGrid() const noexcept;

		///
		/// Get image.
		///
		/// \return String, format: #RRGGBB or #AARRGGBB.
		///
		const std::string& getImage() const noexcept;

		///
		/// Get image height in pixels.
		///
		/// \return Const int.
		///
		const int getImageHeight() const noexcept;

		///
		/// Get image width in pixels.
		///
		/// \return Const int.
		///
		const int getImageWidth() const noexcept;

		///
		/// Get buffer between image edge and first tile.
		///
		/// \return Const int.
		///
		const int getMargin() const noexcept;

		///
		/// Get tileset name.
		///
		/// \return Name as string.
		///
		const std::string& getName() const noexcept;

		///
		/// \brief Retrieve property.
		///
		/// You will need to provide the type when retrieving.
		///
		/// \param name Name of the property to retrieve.
		///
		/// \return Property cast as type.
		///
		template<typename T>
		const T getProperty(const std::string & name) noexcept;

		///
		/// Get array of terrains.
		///
		/// \return Std::vector containing starmap::Terrain.
		///
		const auto& getTerrain() const noexcept;

		///
		/// Get the number of tiles.
		///
		/// \return Const int.
		///
		const int getTileCount() const noexcept;

		///
		/// Get version of Tiled used to save tileset.
		///
		/// \return Const std::string.
		///
		const std::string& getTiledVersion() const noexcept;

		///
		/// Get the maximum height of tiles.
		///
		/// \return Const int.
		///
		const int getTileHeight() const noexcept;

		///
		/// Get the tile offset used by the tileset.
		///
		/// \return Returns a std::optional. Make sure you check for std::nullopt if tile offset is not used!
		///
		const auto& getTileOffset() const noexcept;

		///
		/// Get the tiles in the tileset.
		///
		/// \return Std::vector array.
		///
		const auto& getTiles() const noexcept;

		///
		/// Get the maximum width of tiles.
		///
		/// \return Const int.
		///
		const int getTileWidth() const noexcept;

		///
		/// Get colour used to mark an area transparent.
		///
		/// \return Const std::string reference. Hex-formatted. Defaults to White (FFFFFF).
		///
		const std::string& getTransparentColour() const noexcept;

		///
		/// Get type of tileset.
		///
		/// \return Const std::string reference. Defaults to "tileset".
		///
		const std::string& getType() const noexcept;

		///
		/// Get wang sets.
		///
		/// \return Std::vector array.
		///
		const auto& getWangSets() const noexcept;

	private:
		///
		/// Hex-formatted color (#RRGGBB or #AARRGGBB) (optional).
		///
		std::string m_backgroundColour;

		///
		/// The number of tile columns in the tileset.
		///
		int m_columns;

		///
		/// GID corresponding to the first tile in the set.
		///
		int m_firstGID;

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
		int m_imageHeight;

		///
		/// Width of source image in pixels
		///
		int m_imageWidth;

		///
		/// Buffer between image edgeand first tile (pixels).
		///
		int m_margin;

		///
		/// Name given to this tileset.
		///
		std::string m_name;

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
		int m_tileCount;

		///
		/// The Tiled version used to save the file.
		///
		std::string m_tiledVersion;
		
		///
		/// Maximum height of tiles in this set.
		///
		int m_tileHeight;
		
		///
		/// Offset in pixels, to be applied when drawing a tile from the related tileset
		///
		std::optional<starmap::TileOffset> m_tileOffset;

		///
		/// Array of tiles.
		///
		std::vector<starmap::Tile> m_tiles;

		///
		/// Maximum width of tiles in this set.
		///
		int m_tileWidth;

		///
		/// Hex - formatted color(#RRGGBB) (optional)
		///
		std::string m_transparentColour;

		///
		/// Tileset(for tileset files, since 1.0).
		///
		std::string m_type;

		///
		/// Array of Wang sets.
		///
		std::vector<starmap::WangSet> m_wangSets;
	};

	template<typename T>
	inline const T Tileset::getProperty(const std::string& name) noexcept
	{
		return m_properties[name].get<T>();
	}
}

#endif