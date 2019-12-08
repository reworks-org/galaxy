///
/// Map.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_MAP_HPP_
#define STARMAP_MAP_HPP_

#include <string_view>

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
		/// Parse (load) a json formatted Tiled map.
		///
		/// \param map File Path to the map to load from disk.
		///
		bool parse(std::string_view map);

		///
		///
		///
		bool parse(void* data, const std::size_t size);

	private:
	};
}

#endif