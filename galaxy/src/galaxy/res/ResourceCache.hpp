///
/// ResourceCache.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RES_RESOURCECACHE_HPP_
#define GALAXY_RES_RESOURCECACHE_HPP_

#include <robin_hood.h>

#include "galaxy/error/Log.hpp"
#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace res
	{
		///
		/// Container structure for storing resource cache objects in containers.
		///
		struct ResCacheBase
		{
			///
			/// Default constructor.
			///
			ResCacheBase() = default;

			///
			/// Virtual default destructor.
			///
			virtual ~ResCacheBase() = default;
		};

		///
		/// Stores a cache of resources in order to make effective use of memory.
		/// resources can be a texture, sound, script, shader, etc...
		///
		template<meta::not_pointer_or_ref Resource>
		class ResourceCache : public ResCacheBase
		{
		public:
			///
			/// Destructor.
			///
			virtual ~ResourceCache() = default;

			///
			/// Create a resource.
			///
			/// \param name Should be name of resource without path or extension.
			/// \param args Constructor arguments.
			///
			/// \return Const pointer to newly created resource.
			///
			template<typename... Args>
			[[maybe_unused]] Resource* const create(std::string_view name, Args&&... args);

			///
			/// Move a resource into cache.
			///
			/// \param name Should be name of resource without path or extension.
			/// \param resource Move-compatible reference to resource.
			///
			/// \return Const pointer to newly created resource.
			///
			[[maybe_unused]] Resource* const move(std::string_view name, Resource&& resource);

			///
			/// Retrieve a resource.
			///
			/// \param handle The name of the file without path or extension.
			///
			/// \return Returns a pointer to the resource.
			///
			[[nodiscard]] Resource* const get(std::string_view handle);

			///
			/// Clean up.
			///
			virtual void clear() noexcept = 0;

		protected:
			///
			/// Constructor.
			///
			ResourceCache() noexcept = default;

		protected:
			///
			/// Contiguous resource array.
			///
			robin_hood::unordered_map<std::string, Resource> m_resources;

		private:
			///
			/// Copy constructor.
			///
			ResourceCache(const ResourceCache&) = delete;

			///
			/// Move constructor.
			///
			ResourceCache(ResourceCache&&) = delete;

			///
			/// Copy assignment operator.
			///
			ResourceCache& operator=(const ResourceCache&) = delete;

			///
			/// Move assignment operator.
			///
			ResourceCache& operator=(ResourceCache&&) = delete;
		};

		template<meta::not_pointer_or_ref Resource>
		template<typename... Args>
		inline Resource* const ResourceCache<Resource>::create(std::string_view name, Args&&... args)
		{
			const auto str = static_cast<std::string>(name);
			m_resources.emplace(
			    std::piecewise_construct, std::make_tuple(str), std::make_tuple(Resource {std::forward<Args>(args)...}));

			return &m_resources[str];
		}

		template<meta::not_pointer_or_ref Resource>
		inline Resource* const ResourceCache<Resource>::move(std::string_view name, Resource&& resource)
		{
			const auto str   = static_cast<std::string>(name);
			m_resources[str] = std::move(resource);

			return &m_resources[str];
		}

		template<meta::not_pointer_or_ref Resource>
		inline Resource* const ResourceCache<Resource>::get(std::string_view name)
		{
			const auto str = static_cast<std::string>(name);
			if (m_resources.contains(str))
			{
				return &m_resources[str];
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Failed to find resource with name: " + str);
				return nullptr;
			}
		}
	} // namespace res
} // namespace galaxy

#endif