///
/// TiledProperty.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MAP_TILEDPROPERTY_HPP_
#define GALAXY_MAP_TILEDPROPERTY_HPP_

#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace map
	{
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