///
/// ResourceCache.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_RESOURCECACHE_HPP_
#define SUPERCLUSTER_RESOURCECACHE_HPP_

#include <unordered_map>

#include "sc/utility/Meta.hpp"

///
/// Core namespace.
///
namespace sc
{
	///
	/// Stores a cache of resources in order to make effective use of memory.
	/// Resources can be a texture, sound, script, shader, etc...
	///
	template<typename Resource>
	class ResourceCache
	{
	public:
		///
		/// Virtual default destructor.
		///
		virtual ~ResourceCache() noexcept = default;

		///
		/// Add a resource.
		///
		template<typename ...Args>
		void add(const std::string& id, Args&& ...args);

		///
		/// Retrieve a resource.
		///
		/// \param id The ID of the resource to retrieve.
		///
		/// \return Returns a pointer to the resource.
		///
		typename sc::ReturnReferenceIfFalse<std::is_pointer<Resource>::value, Resource>::type get(const std::string& id);

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
		std::unordered_map<const char*, Resource> m_resourceMap;
	};

	template<typename Resource>
	inline typename sc::ReturnReferenceIfFalse<std::is_pointer<Resource>::value, Resource>::type ResourceCache<Resource>::get(const std::string& id)
	{
		return m_resourceMap[id.c_str()];
	}

	template<typename Resource>
	template<typename ...Args>
	inline void ResourceCache<Resource>::add(const std::string& id, Args&& ...args)
	{
		m_resourceMap.emplace(id.c_str(), std::forward<Args>(args)...);
	}
}

#endif