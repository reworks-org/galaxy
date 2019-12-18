///
/// Tile.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_TILE_HPP_
#define STARMAP_TILE_HPP_

#include <optional>

#include "starmap/types/Frame.hpp"
#include "starmap/types/Property.hpp"

///
/// Core namespace.
///
namespace starmap
{
	///
	/// Forward dec of ObjectLayer.
	///
	class ObjectLayer;

	///
	/// A tile on a tileset.
	///
	class Tile final
	{
	public:
		///
		/// Default constructor.
		///
		Tile() noexcept;

		///
		/// \brief Parse constructor.
		///
		/// Can throw exceptions.
		///
		/// \param json JSON structure/array containing tile json.
		///
		explicit Tile(const nlohmann::json& json);

		///
		/// Copy constructor.
		///
		Tile(const Tile&) = default;

		///
		/// Move constructor.
		///
		Tile(Tile&&) = default;

		///
		/// Destructor.
		///
		~Tile();

		///
		/// \brief Parses json structure to member values; etc.
		///
		/// Can throws exceptions.
		///
		/// \param json JSON structure containing tile json.
		///
		void parse(const nlohmann::json& json);

		///
		/// Get animation frames.
		///
		/// \return Std::vector array.
		///
		const auto& getAnimations() const noexcept;

		///
		/// Get local tile id.
		///
		/// \return Const int.
		///
		const int getID() const noexcept;

		///
		/// Get image representing the tile.
		///
		/// \return Const std::string reference.
		///
		const std::string& getImage() const noexcept;

		///
		/// Height of the tile.
		///
		/// \return Const int. In pixels.
		///
		const int getImageHeight() const noexcept;
		
		///
		/// Width of the tile.
		///
		/// \return Const int. In pixels.
		///
		const int getImageWidth() const noexcept;

		///
		/// Get the object group of the Tile.
		///
		/// \return Returns a std::optional. Make sure you check for std::nullopt if tile offset is not used!
		///
		const auto& getObjectGroup() const noexcept;

		///
		/// Chance this tile is chosen when competing with others in the editor.
		///
		/// \return Const double. This is a percentage. Will return -1.0 if not used.
		///
		const double getProbability() const noexcept;

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
		const T getProperty(const std::string& name) noexcept;

		///
		/// Get index of terrain for each corner of tile.
		///
		/// \return std::vector int array.
		///
		const auto& getTerrainIndices() const noexcept;

		///
		/// Get the type of tile.
		///
		/// \return Const std::string reference.
		///
		const std::string& getType() const noexcept;

	private:
		///
		/// Array of Frames.
		///
		std::vector<starmap::Frame> m_animation;

		///
		/// Local ID of the tile.
		///
		int m_id;
		
		///
		/// Image representing this tile (optional).
		///
		std::string m_image;
		
		///
		/// Height of the tile image in pixels.
		///
		int m_imageHeight;

		///
		/// Width of the tile image in pixels.
		///
		int m_imageWidth;

		///
		/// Layer with type objectgroup, when collision shapes are specified (optional).
		///
		std::optional<starmap::ObjectLayer> m_objectGroup;

		///
		/// Percentage chance this tile is chosen when competing with others in the editor (optional).
		///
		double m_probability;

		///
		/// Map of Properties.
		///
		std::unordered_map<std::string, starmap::Property> m_properties;

		///
		/// Index of terrain for each corner of tile (optional).
		///
		std::vector<int> m_terrainIndices;

		///
		/// The type of the tile (optional).
		///
		std::string m_type;
	};

	template<typename T>
	inline const T Tile::getProperty(const std::string& name) noexcept
	{
		return m_properties[name].get<T>();
	}
}

#endif