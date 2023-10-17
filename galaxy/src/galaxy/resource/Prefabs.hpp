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

namespace galaxy
{
	namespace resource
	{
		///
		/// Resource manager for prefabs.
		///
		class PrefabLoader final
		{
		  public:
			///
			/// Overloaded operator() used to load a resource.
			///
			/// \param file Path on disk to load file from. You don't need to check with the filesystem, already done by the cache.
			///
			/// \return Shared pointer to newly created resource.
			///
			std::shared_ptr<core::Prefab> operator()(const std::string& file);
		};

		///
		/// Abbreviation for prefab cache type.
		///
		using Prefabs = Cache<core::Prefab, PrefabLoader, false>;
	} // namespace resource
} // namespace galaxy

#endif
