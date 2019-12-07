///
/// ResourceCache.hpp
/// galaxy
///
/// Created by reworks on 17/12/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCECACHE_HPP_
#define GALAXY_RESOURCECACHE_HPP_

#include <unordered_map>

#include "galaxy/utils/Utils.hpp"
#include "galaxy/libs/entt/core/hashed_string.hpp"

galaxy
{
	///
	/// Stores a cache of resources in order to make effective use of memory.
	/// Resources can be i.e. ALLEGRO_FONT* or Sound, etc.
	///
	template<typename Resource>
	class ResourceCache
	{
		friend class Serializer;
	public:
		///
		/// Destructor.
		///
		virtual ~ResourceCache() noexcept = default;

		///
		/// Retrieve a resource.
		///
		/// \param id The ID of the resource to retrieve.
		///
		/// \return Returns a pointer to the resource.
		///
		inline typename galaxyReturnReferenceIfFalse<std::is_pointer<Resource>::value, Resource>::type get(const std::string& id)
		{
			// Create hashed string.
			entt::HashedString hs(id.c_str());

			// at() is used because it throws an exception.
			return m_resourceMap.at(hs);
		}

		///
		/// Clean up resources.
		///
		virtual void clean() = 0;

	protected:
		///
		/// Default constructor.
		///
		ResourceCache() = default;

		///
		/// Copy Constructor.
		/// Deleted.
		///
		ResourceCache(const ResourceCache&) = delete;

		///
		/// Move Constructor.
		/// Deleted.
		///
		ResourceCache(ResourceCache&&) = delete;

	protected:
		///
		/// The hashmap containing the resources.
		///
		std::unordered_map<entt::HashedString::hash_type, Resource> m_resourceMap;
	};
}

#endif