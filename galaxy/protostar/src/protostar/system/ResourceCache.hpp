///
/// ResourceCache.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_RESOURCECACHE_HPP_
#define PROTOSTAR_RESOURCECACHE_HPP_

#include <unordered_map>

#include <pulsar/Log.hpp>

#include "protostar/utility/Meta.hpp"

///
/// Core namespace.
///
namespace protostar
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
		void add(const std::string& id, Args&& ...args) noexcept;

		///
		/// Retrieve a resource.
		///
		/// \param id The ID of the resource to retrieve.
		///
		/// \return Returns a pointer to the resource.
		///
		typename protostar::ReturnReferenceIfFalse<std::is_pointer<Resource>::value, Resource>::type get(const std::string& id) noexcept;

		///
		/// Clean up resources.
		///
		virtual void clean() noexcept = 0;

	protected:
		///
		/// Default constructor.
		///
		ResourceCache() noexcept = default;

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

	protected:
		///
		/// The hashmap containing the resources.
		///
		std::unordered_map<std::string, Resource> m_resourceMap;
	};

	template<typename Resource>
	inline typename protostar::ReturnReferenceIfFalse<std::is_pointer<Resource>::value, Resource>::type ResourceCache<Resource>::get(const std::string& id) noexcept
	{
		if (m_resourceMap.find(id) != m_resourceMap.end())
		{
			return m_resourceMap[id.c_str()];
		}
		else
		{
			PL_LOG(PL_WARNING, "Tried to access resource that does not exist: " + id);
			return NULL; // nullptr?
		}
	}

	template<typename Resource>
	template<typename ...Args>
	inline void ResourceCache<Resource>::add(const std::string& id, Args&& ...args) noexcept
	{
		if (m_resourceMap.find(id) != m_resourceMap.end())
		{
			m_resourceMap.emplace(id, std::forward<Args>(args)...);
		}
		else
		{
			PL_LOG(PL_WARNING, "Tried to emplace duplicate resource: " + id);
		}
	}
}

#endif