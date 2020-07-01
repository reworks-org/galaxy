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
#include "protostar/system/ResCacheBase.hpp"

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
	class ResourceCache : public protostar::ResCacheBase
	{
	public:
		///
		/// Virtual default destructor.
		///
		virtual ~ResourceCache() noexcept = default;

		///
		/// Add a resource.
		///
		/// \param args Constructor arguments.
		///
		/// \return Handle to created resource.
		///
		template<typename ...Args>
		const unsigned int add(Args&& ...args) noexcept;

		///
		/// Move an existing resource into the cache.
		///
		/// \param res Move constructable resource.
		///
		/// \return Handle to newly added resource.
		///
		virtual const unsigned int move(Resource& resource) noexcept final;

		///
		/// Retrieve a resource.
		///
		/// \param handle The ID of the resource to retrieve.
		///
		/// \return Returns a pointer to the resource.
		///
		virtual Resource* get(const unsigned int handle) noexcept final;

		///
		/// Clean up.
		///
		virtual void clear() noexcept = 0;

	protected:
		///
		/// Default constructor.
		///
		ResourceCache() noexcept;

		///
		/// Copy Constructor.
		///
		ResourceCache(const ResourceCache&) noexcept = default;

		///
		/// Move Constructor.
		///
		ResourceCache(ResourceCache&&) noexcept = default;

	protected:
		///
		/// ID counter.
		///
		unsigned int m_counter;

		///
		/// Contiguous resource array.
		///
		std::vector<Resource> m_resources;
	};
	
	template<typename Resource>
	inline const unsigned int ResourceCache<Resource>::move(Resource& resource) noexcept
	{
		m_resources.emplace(m_resources.begin() + m_counter, std::move(resource));
		m_counter++;

		return m_counter - 1;
	}

	template<typename Resource>
	inline Resource* ResourceCache<Resource>::get(const unsigned int handle) noexcept
	{
		if (handle >= m_resources.size())
		{
			PL_LOG(PL_ERROR, "Handle is out of bounds for cache.");
			return nullptr;
		}
		else
		{
			return &m_resources[handle];
		}
	}

	template<typename Resource>
	inline ResourceCache<Resource>::ResourceCache() noexcept
		:m_counter(0)
	{
	}

	template<typename Resource>
	template<typename ...Args>
	inline const unsigned int ResourceCache<Resource>::add(Args&& ...args) noexcept
	{
		m_resources.emplace(m_resources.begin() + m_counter, std::forward<Args>(args)...);
		m_counter++;

		// Because we increment so to get correct counter we need to decrement.
		return m_counter - 1;
	}
}

#endif