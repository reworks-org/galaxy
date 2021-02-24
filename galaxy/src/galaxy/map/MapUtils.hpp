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
		/// Convert a hex string colour to a graphics colour object.
		///
		/// \param hex_colour String of hex colour i.e. AARRGGBB or RRGGBB.
		///
		/// \return Newly constructed graphics::Colour object.
		///
		graphics::Colour parse_hex_colour(std::string_view hex_colour);

		///
		/// Concept for tiled property types.
		///
		template<typename Type>
		concept tiled_property = (std::is_same<Type, std::string>::value ||
					  std::is_same<Type, int>::value ||
					  std::is_same<Type, float>::value ||
					  std::is_same<Type, bool>::value);
	} // namespace map
} // namespace galaxy

#endif