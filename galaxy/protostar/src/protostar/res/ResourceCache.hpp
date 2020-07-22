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

#include "protostar/system/Concepts.hpp"

///
/// Core namespace.
///
namespace pr
{
	///
	/// Container structure for storing resourceCache objects in containers.
	///
	class ResCacheBase
	{
	public:
		///
		/// Default constructor.
		///
		ResCacheBase() noexcept = default;

		///
		/// Virtual default destructor.
		///
		virtual ~ResCacheBase() noexcept = default;
	};

	///
	/// Stores a cache of resources in order to make effective use of memory.
	/// resources can be a texture, sound, script, shader, etc...
	///
	template<no_pointer_or_ref resource>
	class ResourceCache : public pr::ResCacheBase
	{
	public:
		///
		/// Virtual destructor.
		///
		virtual ~ResourceCache() noexcept;

		///
		/// Create a resource.
		///
		/// \param name Should be name of resource without path or extension.
		/// \param args Constructor arguments.
		///
		/// \return Pointer to newly created resource.
		///
		template<typename... _args>
		[[nodiscard]] resource* create(std::string_view name, _args&&... args);

		///
		/// Move a resource into cache.
		///
		/// \param name Should be name of resource without path or extension.
		/// \param resource Move-compatible reference to resource.
		///
		/// \return Pointer to newly created resource.
		///
		[[maybe_unused]] virtual resource* move(std::string_view name, resource& resource) noexcept final;

		///
		/// Retrieve a resource.
		///
		/// \param handle The name of the file without path or extension.
		///
		/// \return Returns a pointer to the resource.
		///
		[[nodiscard]] virtual resource* get(std::string_view handle) noexcept final;

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

		///
		/// Contiguous resource array.
		///
		std::unordered_map<std::string, resource> m_resources;
	};

	template<no_pointer_or_ref resource>
	inline ResourceCache<resource>::~ResourceCache() noexcept
	{
		clear();
	}

	template<no_pointer_or_ref resource>
	template<typename... _args>
	inline resource* ResourceCache<resource>::create(std::string_view name, _args&&... args)
	{
		const auto str = static_cast<std::string>(name);
		m_resources.emplace(
		    std::piecewise_construct, std::make_tuple(str), std::make_tuple(std::forward<_args>(args)...));

		return &m_resources[str];
	}

	template<no_pointer_or_ref resource>
	inline resource* ResourceCache<resource>::move(std::string_view name, resource& resource) noexcept
	{
		const auto str   = static_cast<std::string>(name);
		m_resources[str] = std::move(resource);

		return &m_resources[str];
	}

	template<no_pointer_or_ref resource>
	inline resource* ResourceCache<resource>::get(std::string_view name) noexcept
	{
		const auto str = static_cast<std::string>(name);
		if (m_resources.contains(str))
		{
			return &m_resources[str];
		}
		else
		{
			PL_LOG(PL_WARNING, "Failed to find resource with name: " + str);
			return nullptr;
		}
	}

	template<no_pointer_or_ref resource>
	inline ResourceCache<resource>::ResourceCache() noexcept
	{
	}
} // namespace pr

#endif