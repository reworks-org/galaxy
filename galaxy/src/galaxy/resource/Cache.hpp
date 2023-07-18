///
/// Cache.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_CACHE_HPP_
#define GALAXY_RESOURCE_CACHE_HPP_

#include <BS_thread_pool.hpp>
#include <robin_hood.h>

#include "galaxy/meta/Memory.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Provides a useful set of functions around managing a resource cache.
		///
		/// \tparam Resource A resource can be anything that is not a pointer or a reference.
		///
		template<meta::not_memory Resource>
		class Cache
		{
			///
			/// Defines the template syntax for a resource holder.
			///
			using CacheMap = robin_hood::unordered_node_map<std::string, std::shared_ptr<Resource>>;

		public:
			///
			/// Destructor.
			///
			virtual ~Cache();

			///
			/// Clean up resources.
			///
			void clear();

			///
			/// Loads resources from a folder.
			///
			/// \param folder Folder located in the VFS.
			///
			/// \return Thread handle of loading thread.
			///
			virtual std::future<void> load(std::string_view folder) = 0;

			///
			/// Check if a resource exists.
			///
			/// \param key Key to use to access resource.
			///
			/// \return True if resource was found.
			///
			[[nodiscard]] bool has(const std::string& key);

			///
			/// Retrieve a resource.
			///
			/// \param key Key to use to access resource.
			///
			/// \return Returns a pointer to the resource.
			///
			[[nodiscard]] std::shared_ptr<Resource> get(const std::string& key);

			///
			/// Check if the cache has any resources in it.
			///
			/// \return True if empty, false otherwise.
			///
			[[nodiscard]] bool empty() const;

			///
			/// Get entire resource cache.
			///
			/// \return Reference to the resource holders cache.
			///
			[[nodiscard]] CacheMap& cache();

			///
			/// Get a list of keys in the cache.
			///
			/// \return A vector of strings.
			///
			[[nodiscard]] meta::vector<std::string> keys();

		protected:
			///
			/// Constructor.
			///
			Cache() = default;

		protected:
			///
			/// Contiguous resource array.
			///
			CacheMap m_cache;

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
		};

		template<meta::not_memory Resource>
		inline Cache<Resource>::~Cache()
		{
			clear();
		}

		template<meta::not_memory Resource>
		inline void Cache<Resource>::clear()
		{
			m_cache.clear();
		}

		template<meta::not_memory Resource>
		inline bool Cache<Resource>::has(const std::string& key)
		{
			return m_cache.contains(key);
		}

		template<meta::not_memory Resource>
		inline std::shared_ptr<Resource> Cache<Resource>::get(const std::string& key)
		{
			if (has(key))
			{
				return m_cache[key];
			}

			return nullptr;
		}

		template<meta::not_memory Resource>
		inline bool Cache<Resource>::empty() const
		{
			return m_cache.empty();
		}

		template<meta::not_memory Resource>
		inline Cache<Resource>::CacheMap& Cache<Resource>::cache()
		{
			return m_cache;
		}

		template<meta::not_memory Resource>
		inline meta::vector<std::string> Cache<Resource>::keys()
		{
			meta::vector<std::string> keys;
			keys.reserve(m_cache.size());

			for (const auto& [key, _] : m_cache)
			{
				keys.push_back(key);
			}

			return keys;
		}
	} // namespace resource
} // namespace galaxy

#endif