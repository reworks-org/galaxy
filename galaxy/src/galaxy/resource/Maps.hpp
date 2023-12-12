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

namespace galaxy
{
	namespace resource
	{
		///
		/// Resource manager for tiled maps.
		///
		class MapLoader final
		{
		  public:
			///
			/// Overloaded operator() used to load a resource.
			///
			/// \param file Path on disk to load file from. You don't need to check with the filesystem, already done by the cache.
			///
			/// \return Shared pointer to newly created resource.
			///
			std::shared_ptr<map::Map> operator()(const std::string& file);
		};

		///
		/// Abbreviation for map cache type.
		///
		using Maps = Cache<map::Map, MapLoader, false>;
	} // namespace resource
} // namespace galaxy

#endif
