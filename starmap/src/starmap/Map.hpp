///
/// Map.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_MAP_HPP_
#define STARMAP_MAP_HPP_

#include <nlohmann/json.hpp>

#include "starmap/tile/Tileset.hpp"

///
/// Core namespace.
///
namespace starmap
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
		Map() noexcept;

		///
		/// Destructor.
		///
		~Map() noexcept;

		///
		/// Load a json formatted Tiled map.
		///
		/// \param map File Path to the map to load from disk.
		///
		/// \return True if successful.
		///
		bool load(const std::string& map);

		///
		/// Load a json file from memory.
		///
		/// \param buffer Pointer to buffer. IS NOT FREED, YOU MUST FREE AFTER CALLING THIS FUNCTION.
		/// \param size Size of the buffer.
		///
		/// \return True if successful.
		///
		bool load(char* buffer, const std::size_t size);

		///
		/// \brief Parses json structure to member values, etc.
		///
		/// You can surround this with a try-catch block since it does throw exceptions.
		///
		void parse();

		///
		/// Get background colour of map.
		///
		/// \return String, format: #RRGGBB or #AARRGGBB
		///
		const std::string& getBackgroundColour() const noexcept;

		///
		/// Get number of tile rows.
		///
		/// \return Const int.
		///
		const int getHeight() const noexcept;

		///
		/// \brief Get length of the side of a hex tile.
		///
		/// Only hexagonal maps.
		///
		/// \return Const int. In pixels.
		///
		const int getHexSideLength() const noexcept;

		///
		/// Does the map have infinite dimensions.
		///
		/// \return True if is infinite.
		///
		const bool isInfinite() const noexcept;

		///
		/// Get map layers.
		///
		/// \return Std::vector of unique_ptrs containing polymorphic layers.
		///
		const auto& getLayers() const noexcept;

		///
		/// Returns the next free ID for the creation of a layer.
		///
		/// \return Const int.
		///
		const int getNextLayerID() const noexcept;

		///
		/// Returns the next free ID for the creation of an object.
		///
		/// \return Const int.
		///
		const int getNextObjectID() const noexcept;

		///
		/// Map viewpoint orientation.
		///
		/// \return String, format: orthogonal, isometric, staggered or hexagonal.
		///
		const std::string& getOrientation() const noexcept;

		///
		/// Retrieve property.
		/// You will need to provide the type when retrieving.
		///
		/// \param name Name of the property to retrieve.
		///
		/// \return Property cast as type. const.
		///
		template<typename T>
		const T getProperty(const std::string& name) noexcept;

		///
		/// \brief Defines order to render tiles in.
		///
		/// Orthogonal maps only.
		///
		/// \return String, format: right-down, right-up, left-down or left-up.
		///
		const std::string& getRenderOrder() const noexcept;

		///
		/// Get stagger axis.
		///
		/// \return String, format: x or y.
		///
		const std::string& getStaggerAxis() const noexcept;

		///
		/// Get stagger index.
		///
		/// \return String, format: odd or even.
		///
		const std::string& getStaggerIndex() const noexcept;

		///
		/// Get tiled version used to save the file.
		///
		/// \return Formatted const string.
		///
		const std::string& getTiledVersion() const noexcept;

		///
		/// Get the map grid height.
		///
		/// \return Const int.
		///
		const int getTileHeight() const noexcept;

		///
		/// Get the array of tilesets.
		///
		/// \return Std::vector of tilesets.
		///
		const auto& getTileSets() const noexcept;

		///
		/// Get the map grid width.
		///
		/// \return Const int.
		///
		const int getTileWidth() const noexcept;

		///
		/// Get type of tiled file.
		///
		/// \return String, format: map.
		///
		const std::string& getType() const noexcept;

		///
		/// Gets the number of tile columns.
		///
		/// \return Const int.
		///
		const int getWidth() const noexcept;

	private:
		///
		/// Flag for seeing if json is loaded.
		///
		bool m_isLoaded;

		///
		/// Maser root node of map file.
		///
		nlohmann::json m_root;

		///
		/// Hex-formatted colour (#RRGGBB or #AARRGGBB) (optional).
		///
		std::string m_backgroundColour;

		///
		/// Number of tile rows.
		///
		int m_height;

		///
		/// Length of the side of a hex tile in pixels (hexagonal maps only).
		///
		int m_hexSideLength;

		///
		/// Whether the map has infinite dimensions.
		///
		bool m_infinite;

		///
		/// Array of Layers.
		///
		std::vector<std::unique_ptr<starmap::Layer>> m_layers;

		///
		/// Auto-increments for each layer.
		///
		int m_nextLayerID;

		///
		/// Auto-increments for each placed object.
		///
		int m_nextObjectID;

		///
		/// Map viewpoint orientation.
		/// orthogonal, isometric, staggered or hexagonal.
		///
		std::string m_orientation;

		///
		/// Map of Properties.
		///
		std::unordered_map<std::string, starmap::Property> m_properties;

		///
		/// Right-down (the default), right-up, left-down or left-up (orthogonal maps only).
		///
		std::string m_renderOrder;

		///
		/// X or y (staggered / hexagonal maps only).
		///
		std::string m_staggerAxis;

		///
		/// Odd or even (staggered / hexagonal maps only);
		///
		std::string m_staggerIndex;

		///
		/// The Tiled version used to save the file.
		///
		std::string m_tiledVersion;
	
		///
		/// Map grid height.
		///
		int m_tileHeight;

		///
		/// Array of Tilesets.
		///
		std::vector<starmap::Tileset> m_tileSets;

		///
		/// Map grid width.
		///
		int m_tileWidth;

		///
		/// Type of tiled file.
		///
		std::string m_type;

		///
		/// Number of tile columns.
		///
		int m_width;
	};

	template<typename T>
	inline const T Map::getProperty(const std::string& name) noexcept
	{
		return m_properties[name].get<T>();
	}
}

#endif