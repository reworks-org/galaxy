///
/// Maps.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_MAPS_HPP_
#define GALAXY_RESOURCE_MAPS_HPP_

#include "galaxy/map/Map.hpp"
#include "galaxy/resource/Cache.hpp"
#include "galaxy/resource/Loader.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Abbreviation for map cache.
		///
		using Maps = Cache<map::Map, Loader<map::Map>>;
	} // namespace resource
} // namespace galaxy

#endif
