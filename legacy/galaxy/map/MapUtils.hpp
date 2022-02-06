///
/// MapUtils.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MAP_MAPUTILS_HPP_
#define GALAXY_MAP_MAPUTILS_HPP_

#include "galaxy/graphics/Colour.hpp"
#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace map
	{
		///
		/// Horizontal Flip Flag.
		///
		constexpr const auto FLIPPED_HORIZONTALLY_FLAG = 0x80000000;

		///
		/// Vertical Flip Flag.
		///
		constexpr const auto FLIPPED_VERTICALLY_FLAG = 0x40000000;

		///
		/// Diagonal Flip Flag.
		///
		constexpr const auto FLIPPED_DIAGONALLY_FLAG = 0x20000000;

		///
		/// Concept for tiled property types.
		///
		template<typename Type>
		concept tiled_property = (std::is_same<Type, std::string>::value || std::is_same<Type, int>::value || std::is_same<Type, float>::value ||
								  std::is_same<Type, bool>::value || std::is_same<Type, graphics::Colour>::value);

		///
		/// Convert a hex string colour to a graphics colour object.
		///
		/// \param hex_colour String of hex colour i.e. AARRGGBB or RRGGBB.
		///
		/// \return Newly constructed graphics::Colour object.
		///
		[[nodiscard]] graphics::Colour parse_hex_colour(std::string_view hex_colour);

		///
		/// Get tile flags from a tile global id.
		///
		/// \param gid Tile Global ID.
		///
		/// \return Returns horizontal as index 0, vertical as index 1 and diagonal as index 2.
		///
		[[nodiscard]] std::tuple<bool, bool, bool> get_tile_flags(const int gid) noexcept;

		///
		/// Unset tile flags.
		///
		/// \param gid Tile Global ID.
		///
		/// \return "Clean" tile global id.
		///
		[[nodiscard]] const int unset_tile_flags(const int gid) noexcept;
	} // namespace map
} // namespace galaxy

#endif