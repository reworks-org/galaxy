///
/// Loader.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_LOADER_HPP_
#define GALAXY_RESOURCE_LOADER_HPP_

#include <entt/core/hashed_string.hpp>
#include <entt/resource/cache.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/meta/Memory.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Loads resources for a resource cache.
		///
		/// \tparam Resource A resource is a class containing data.
		///
		template<typename Resource>
		requires meta::not_memory<Resource> && meta::is_class<Resource>
		struct Loader
		{
			///
			/// Overloaded operator() used to load a resource.
			///
			/// \param file Path on disk/archive to load file from.
			///
			/// \return Handle to created resource.
			///
			inline std::shared_ptr<Resource> operator()(const std::string& file)
			{
				auto resource = std::make_shared<Resource>();
				if (!resource->load(file))
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to load resource '{0}'.", file);
					return nullptr;
				}

				return resource;
			}
		};
	} // namespace resource
} // namespace galaxy

#endif
