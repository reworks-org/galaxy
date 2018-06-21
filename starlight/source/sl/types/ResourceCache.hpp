///
/// ResourceCache.hpp
/// starlight
///
/// Created by reworks on 17/12/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_RESOURCECACHE_HPP_
#define STARLIGHT_RESOURCECACHE_HPP_

#include <unordered_map>

#include "sl/utils/Utils.hpp"
#include "sl/libs/entt/core/hashed_string.hpp"

namespace sl
{
	///
	/// Stores a cache of resources in order to make effective use of memory.
	/// Resources can be i.e. ALLEGRO_FONT* or Sound, etc.
	///
	template<typename Resource>
	class ResourceCache
	{
	public:
		///
		/// Destructor.
		///
		virtual ~ResourceCache() = default;

		///
		/// Retrieve a resource.
		///
		/// \param id The ID of the resource to retrieve.
		///
		/// \return Returns a pointer to the resource.
		///
		inline typename sl::ReturnReferenceIfFalse<std::is_pointer<Resource>::value, Resource>::type get(const char* id)
		{
			// Create hashed string.
			entt::HashedString hs{ id };
			if (m_resourceMap.find(hs) != m_resourceMap.end())
			{
				return m_resourceMap[hs];
			}
			else
			{
				LOG_S(FATAL) << "Attempted to access a non-existant resource: " << id;
				return m_resourceMap.at(hs); // at() is used here because it throws an exception if it does not exist.
			}
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