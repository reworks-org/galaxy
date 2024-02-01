///
/// Prefabs.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_PREFABS_HPP_
#define GALAXY_RESOURCE_PREFABS_HPP_

#include "galaxy/core/Prefab.hpp"
#include "galaxy/resource/Cache.hpp"
#include "galaxy/resource/Loader.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Abbreviation for prefab cache.
		///
		using Prefabs = Cache<core::Prefab, Loader<core::Prefab>>;
	} // namespace resource
} // namespace galaxy

#endif
