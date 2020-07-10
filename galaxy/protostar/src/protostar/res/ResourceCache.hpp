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
		/// Virtual destructor.
		///
		virtual ~ResourceCache() noexcept;

		///
		/// Add a resource.
		///
		/// \param name Should be name of resource without path or extension.
		/// \param args Constructor arguments.
		///
		template<typename ...Args>
		void add(const std::string& name, Args&& ...args) noexcept;

		//
		// Move
		//

		///
		/// Retrieve a resource.
		///
		/// \param handle The name of the file without path or extension.
		///
		/// \return Returns a pointer to the resource.
		///
		virtual Resource* get(std::string_view handle) noexcept final;

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
		/// Contiguous resource array.
		///
		std::unordered_map<std::string, Resource> m_resources;
	};

	template<typename Resource>
	inline ResourceCache<Resource>::ResourceCache() noexcept
	{
	}

	template<typename Resource>
	inline ResourceCache<Resource>::~ResourceCache() noexcept
	{
		m_resources.clear();
	}

	template<typename Resource>
	template<typename ...Args>
	inline void ResourceCache<Resource>::add(const std::string& name, Args&& ...args) noexcept
	{
		m_resources.emplace(std::piecewise_construct, std::make_tuple(name), std::make_tuple(std::forward<Args>(args)...));
	}

	template<typename Resource>
	inline Resource* ResourceCache<Resource>::get(std::string_view handle) noexcept
	{
		const std::string cast = static_cast<std::string>(handle);
		return &m_resources[cast];
	}
}

#endif