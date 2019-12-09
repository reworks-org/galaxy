///
/// Map.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_MAP_HPP_
#define STARMAP_MAP_HPP_

#include "starmap/types/Layer.hpp"

///
/// Core namespace.
///
namespace starmap
{
	///
	/// Represents the entire Tiled Map.
	///
	class Map
	{
	public:
		///
		/// Default constructor.
		///
		Map() noexcept;

		///
		/// Copy constructor.
		///
		Map(const Map&) noexcept = default;

		///
		/// Move constructor.
		///
		Map(Map&&) noexcept = default;

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
		std::vector<starmap::Layer> m_layers;
	};
}

#endif