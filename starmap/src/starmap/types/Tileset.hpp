///
/// Tileset.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_TILESET_HPP_
#define STARMAP_TILESET_HPP_

#include <optional>

#include "nlohmann/json_fwd.hpp"
#include "starmap/types/Grid.hpp"

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
	};
}

#endif