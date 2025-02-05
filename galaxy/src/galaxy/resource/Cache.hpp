///
/// Cache.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_CACHE_HPP_
#define GALAXY_RESOURCE_CACHE_HPP_

#include <ankerl/unordered_dense.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/math/FNV1a.hpp"
#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Cache for resources such as audio, fonts, etc.
		///
		/// \tparam Resource Type of resource to manage.
		/// \tparam Loader Loader to use when loading the resource.
		///
		template<meta::not_memory Resource, meta::not_memory Loader>
		// requires meta::is_class < Resource>&& meta::is_loader < Loader, Resource>
		class Cache final
		{
			using CacheType = ankerl::unordered_dense::map<std::uint64_t, std::unique_ptr<Resource>>;

		public:
			///
			/// Constructor.
			///
			inline Cache() noexcept
			{
			}

			///
			/// Destructor.
			///
			inline ~Cache()
			{
				clear();
			}

			///
			/// Load a resource.
			///
			/// \param file File to load. Also serves as key hash.
			///
			inline void load(const std::string& file)
			{
				m_keys.push_back(file);
				m_cache[hash(file)] = std::move(m_loader(file));
			}

			///
			/// Load resources of a specific type.
			///
			/// \param dir The directory in the vfs to load assets from.
			///
			inline void load_folder(const std::string& dir)
			{
				clear();

				for (const auto& file : core::ServiceLocator<fs::VirtualFileSystem>::ref().list(dir))
				{
					load(file);
				}
			}

			///
			/// Insert a resource directly into the cache.
			///
			/// \param id Key to use to access resource.
			/// \param resource Pointer to resource to take ownership of.
			///
			inline void insert(std::string_view id, std::unique_ptr<Resource>& resource)
			{
				const auto hashed = hash(id);
				if (!m_cache.contains(hashed))
				{
					m_cache[hashed] = std::move(resource);
				}
				else
				{
					GALAXY_LOG(GALAXY_WARNING, "Tried to load duplicate resource '{0}'.", id);
				}
			}

			///
			/// Retrieve a resource.
			///
			/// \param id Key to use to access resource.
			///
			/// \return Returns a shared pointer to the resource.
			///
			[[nodiscard]]
			inline Resource* get(std::string_view id)
			{
				const auto hashed = hash(id);

				if (m_cache.contains(hashed))
				{
					return m_cache[hashed].get();
				}

				return nullptr;
			}

			///
			/// Destroy resources.
			///
			inline void clear()
			{
				m_cache.clear();
			}

			///
			/// Check if a resource exists.
			///
			/// \param id Key to use to access resource.
			///
			/// \return True if resource was found.
			///
			[[nodiscard]]
			inline bool has(std::string_view id)
			{
				return m_cache.contains(hash(id));
			}

			///
			/// Does the cache have any resources.
			///
			/// \return True if empty, false otherwise.
			///
			[[nodiscard]]
			inline bool empty() const
			{
				return size() == 0;
			}

			///
			/// Get amount of resources cached.
			///
			[[nodiscard]]
			inline std::size_t size() const
			{
				return m_cache.size();
			}

			///
			/// Get entire resource cache.
			///
			/// \return Const reference to the resource cache.
			///
			[[nodiscard]]
			inline const CacheType& cache() const
			{
				return m_cache;
			}

			///
			/// Get a list of keys in the cache.
			///
			/// \return A vector of char pointers.
			///
			[[nodiscard]]
			inline const meta::vector<std::string>& keys()
			{
				return m_keys;
			}

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

			///
			/// Hashes the key into an integer for faster map lookup time.
			///
			inline std::uint64_t hash(std::string_view str)
			{
				const auto hash = math::fnv1a_64(str.data());
				return hash;
			}

		private:
			///
			/// Used to load a resource into the cache. Allows for flexiblity.
			///
			Loader m_loader;

			///
			/// The actual data store.
			///
			CacheType m_cache;

			///
			/// A list of keys currently in the cache.
			///
			meta::vector<std::string> m_keys;
		};
	} // namespace resource
} // namespace galaxy

#endif
