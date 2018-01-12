///
///  ResourceCache.hpp
///  rework
///
///  Created by reworks on 17/12/2017.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef REWORK_RESOURCECACHE_HPP_
#define REWORK_RESOURCECACHE_HPP_

#include <type_traits>
#include <unordered_map>

#include "entt/core/hashed_string.hpp"

namespace re
{
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
		/// \param name The name of the resource to retrieve.
		///
		/// \return Returns a pointer to the resource.
		///
		inline typename std::enable_if<std::is_pointer<Resource>::value, Resource>::type get(entt::HashedString id)
		{
			return m_resourceMap[id];
		}

		///
		/// Retrieve a resource.
		///
		/// \param name The name of the resource to retrieve.
		///
		/// \return Returns a reference to the resource.
		///
		inline typename std::enable_if<!std::is_pointer<Resource>::value, Resource>::type
		get(entt::HashedString id)
		{
			return &(m_resourceMap[id]);
		}

		///
		/// Clean up resources.
		///
		virtual void clean() = 0;

	protected:
		std::unordered_map<entt::HashedString::hash_type, Resource> m_resourceMap;

	protected:
		///
		/// Default constructor.
		///
		ResourceCache() = default;

	private:
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
	};
}

#endif