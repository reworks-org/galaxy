///
/// Cache.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_CACHE_HPP_
#define GALAXY_RESOURCE_CACHE_HPP_

#include <ankerl/unordered_dense.h>
#include <entt/locator/locator.hpp>

#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/logging/Log.hpp"
#include "galaxy/math/FNV1a.hpp"
#include "galaxy/resource/Loader.hpp"

namespace galaxy
{
	///
	/// Cache for resources such as audio, fonts, etc.
	///
	/// \tparam Resource  A resource is a class containing data, and cannot be a ref or ptr.
	/// \tparam SpecLoader A template specialization of a Loader, to load the Resource.
	///
	template<typename Resource, typename SpecLoader>
	requires meta::not_memory<Resource> && meta::is_class<Resource> && meta::is_instance_of_v<SpecLoader, Loader>
	class Cache final
	{
		///
		/// Use a dense map for storage.
		///
		using Map = ankerl::unordered_dense::map<std::uint64_t, std::shared_ptr<Resource>>;

	public:
		///
		/// Constructor.
		///
		Cache() noexcept;

		///
		/// Destructor.
		///
		~Cache();

		///
		/// \brief Load a resource.
		///
		/// Key becomes filename.
		/// This expects the resource to have a compatible string constructor.
		///
		/// \param file File to load in VFS.
		///
		void load_file(const std::string& file);

		///
		/// Loads a resource with custom args.
		///
		/// \tparam Args Argument types.
		///
		/// \param key Access key / resource name.
		/// \param args Forward constructor args.
		///
		template<typename... Args>
		void load_resource(const std::string& key, Args&&... args);

		///
		/// \brief Load all resources in a folder.
		///
		/// Key becomes filename.
		/// This expects the resource to have a compatible string constructor.
		///
		/// \param dir The directory in the vfs to load from.
		///
		void load_folder(const std::string& dir);

		///
		/// Insert a resource directly into the cache.
		///
		/// \param key Access key / resource name.
		/// \param resource Shared pointer to resource.
		///
		void insert(const std::string& key, std::shared_ptr<Resource> resource) noexcept;

		///
		/// Get a resource.
		///
		/// \param key Name/id of resource.
		///
		/// \return Returns a shared pointer to the resource.
		///
		[[nodiscard]]
		std::shared_ptr<Resource> get(const std::string& key) noexcept;

		///
		/// Check if a resource exists.
		///
		/// \param key Name/id of resource.
		///
		/// \return True if resource was found.
		///
		[[nodiscard]]
		bool has(const std::string& key) noexcept;

		///
		/// Destroy all data in cache.
		///
		void clear();

		///
		/// Does the cache have any resources.
		///
		/// \return True if empty, false otherwise.
		///
		[[nodiscard]]
		bool empty() const noexcept;

		///
		/// Get amount of resources cached.
		///
		[[nodiscard]]
		std::size_t size() const noexcept;

		///
		/// Get entire resource cache.
		///
		/// \return Const reference to the resource cache.
		///
		[[nodiscard]]
		const Map& cache() const noexcept;

	private:
		///
		/// Copy constructor.
		///
		Cache(const Cache&) = delete;

		///
		/// Move constructor.
		///
		Cache(Cache&&) = delete;

		///
		/// Copy assignment operator.
		///
		Cache& operator=(const Cache&) = delete;

		///
		/// Move assignment operator.
		///
		Cache& operator=(Cache&&) = delete;

	private:
		///
		/// Flexible Loader used to create/load a Resource into the cache.
		///
		SpecLoader m_loader;

		///
		/// Resource storage map.
		///
		Map m_cache;
	};

	template<typename Resource, typename SpecLoader>
	requires meta::not_memory<Resource> && meta::is_class<Resource> && meta::is_instance_of_v<SpecLoader, Loader>
	inline Cache<Resource, SpecLoader>::Cache() noexcept
	{
	}

	template<typename Resource, typename SpecLoader>
	requires meta::not_memory<Resource> && meta::is_class<Resource> && meta::is_instance_of_v<SpecLoader, Loader>
	inline Cache<Resource, SpecLoader>::~Cache()
	{
		clear();
	}

	template<typename Resource, typename SpecLoader>
	requires meta::not_memory<Resource> && meta::is_class<Resource> && meta::is_instance_of_v<SpecLoader, Loader>
	inline void Cache<Resource, SpecLoader>::load_file(const std::string& file)
	{
		const auto hash = math::fnv1a(file.c_str());
		if (!m_cache.contains(hash))
		{
			m_cache[hash] = m_loader(file);
		}
		else
		{
			GALAXY_LOG(GALAXY_WARN, "Tried to load duplicate resource: '{0}'.", file);
		}
	}

	template<typename Resource, typename SpecLoader>
	requires meta::not_memory<Resource> && meta::is_class<Resource> && meta::is_instance_of_v<SpecLoader, Loader>
	template<typename... Args>
	inline void Cache<Resource, SpecLoader>::load_resource(const std::string& key, Args&&... args)
	{
		const auto hash = math::fnv1a(key.c_str());
		if (!m_cache.contains(hash))
		{
			m_cache[hash] = m_loader(std::forward<Args>(args)...);
		}
		else
		{
			GALAXY_LOG(GALAXY_WARN, "Tried to load duplicate resource: '{0}'.", key);
		}
	}

	template<typename Resource, typename SpecLoader>
	requires meta::not_memory<Resource> && meta::is_class<Resource> && meta::is_instance_of_v<SpecLoader, Loader>
	inline void Cache<Resource, SpecLoader>::load_folder(const std::string& dir)
	{
		auto& fs = entt::locator<VirtualFileSystem>::value();
		for (const auto& file : fs.list(dir))
		{
			load_file(file);
		}
	}

	template<typename Resource, typename SpecLoader>
	requires meta::not_memory<Resource> && meta::is_class<Resource> && meta::is_instance_of_v<SpecLoader, Loader>
	inline void Cache<Resource, SpecLoader>::insert(const std::string& key, std::shared_ptr<Resource> resource) noexcept
	{
		const auto hash = math::fnv1a(key.c_str());
		if (!m_cache.contains(hash))
		{
			m_cache[hash] = resource;
		}
		else
		{
			GALAXY_LOG(GALAXY_WARN, "Tried to load duplicate resource: '{0}'.", key);
		}
	}

	template<typename Resource, typename SpecLoader>
	requires meta::not_memory<Resource> && meta::is_class<Resource> && meta::is_instance_of_v<SpecLoader, Loader>
	inline std::shared_ptr<Resource> Cache<Resource, SpecLoader>::get(const std::string& key) noexcept
	{
		const auto hash = math::fnv1a(key.c_str());
		if (m_cache.contains(hash))
		{
			return m_cache[hash];
		}

		return nullptr;
	}

	template<typename Resource, typename SpecLoader>
	requires meta::not_memory<Resource> && meta::is_class<Resource> && meta::is_instance_of_v<SpecLoader, Loader>
	inline bool Cache<Resource, SpecLoader>::has(const std::string& key) noexcept
	{
		const auto hash = math::fnv1a(key.c_str());
		return m_cache.contains(hash);
	}

	template<typename Resource, typename SpecLoader>
	requires meta::not_memory<Resource> && meta::is_class<Resource> && meta::is_instance_of_v<SpecLoader, Loader>
	inline void Cache<Resource, SpecLoader>::clear()
	{
		m_cache.clear();
	}

	template<typename Resource, typename SpecLoader>
	requires meta::not_memory<Resource> && meta::is_class<Resource> && meta::is_instance_of_v<SpecLoader, Loader>
	inline bool Cache<Resource, SpecLoader>::empty() const noexcept
	{
		return size() == 0;
	}

	template<typename Resource, typename SpecLoader>
	requires meta::not_memory<Resource> && meta::is_class<Resource> && meta::is_instance_of_v<SpecLoader, Loader>
	inline std::size_t Cache<Resource, SpecLoader>::size() const noexcept
	{
		return m_cache.size();
	}

	template<typename Resource, typename SpecLoader>
	requires meta::not_memory<Resource> && meta::is_class<Resource> && meta::is_instance_of_v<SpecLoader, Loader>
	inline const Cache<Resource, SpecLoader>::Map& Cache<Resource, SpecLoader>::cache() const noexcept
	{
		return m_cache;
	}
} // namespace galaxy

#endif
