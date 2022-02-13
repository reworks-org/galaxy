///
/// Cache.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_CACHE_HPP_
#define GALAXY_RESOURCE_CACHE_HPP_

#include "galaxy/error/Log.hpp"
#include "galaxy/resource/Loader.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Validates template input is derived from a loader.
		///
		/// \tparam T Parent Class to check.
		/// \tparam R Resource Loader is using.
		///
		template<typename T, typename R>
		concept is_loader = std::derived_from<T, Loader<R>>;

		///
		/// Stores a cache of resources in order to make effective use of memory.
		/// resources can be a texture, sound, script, shader, etc...
		///
		/// \tparam Resource A resource can be anything that is not a pointer or a reference.
		/// \tparam Loader Loader to use when creating resources.
		///
		template<meta::not_memory Resource, is_loader<Resource> Loader>
		class Cache final
		{
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
			/// Load a resource from a file.
			///
			/// \param key Key to use to access resource.
			/// \param file Path, including filename, to file in the VFS.
			///
			/// \return Pointer to created resource. Null if creation failed.
			///
			[[maybe_unused]] std::shared_ptr<Resource> create(const std::string& key, std::string_view file);

			///
			/// Load a set of resources as defined in a json file.
			///
			/// \param json_file Path, including filename, to file in the VFS.
			///
			void create_from_json(std::string_view json_file);

			///
			/// Clean up resources.
			///
			void clear();

			///
			/// Check if a resource exists.
			///
			/// \param key Key to use to access resource.
			///
			/// \return True if resource was found.
			///
			[[nodiscard]] bool has(const std::string& key) noexcept;

			///
			/// Retrieve a resource.
			///
			/// \param key Key to use to access resource.
			///
			/// \return Returns a pointer to the resource.
			///
			[[nodiscard]] std::shared_ptr<Resource> get(const std::string& key) noexcept;

			///
			/// Check if the cache has any resources in it.
			///
			/// \return True if empty, false otherwise.
			///
			[[nodiscard]] bool empty() const noexcept;

			///
			/// Get entire resource cache.
			///
			/// \return Reference to the resource holders cache.
			///
			[[nodiscard]] Holder<Resource>& cache() noexcept;

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
			/// Used to load the resource into the cache.
			///
			Loader m_loader;

			///
			/// Contiguous resource array.
			///
			Holder<Resource> m_cache;
		};

		template<meta::not_memory Resource, is_loader<Resource> Loader>
		inline Cache<Resource, Loader>::Cache() noexcept
			: m_loader {}
		{
		}

		template<meta::not_memory Resource, is_loader<Resource> Loader>
		inline Cache<Resource, Loader>::~Cache()
		{
			clear();
		}

		template<meta::not_memory Resource, is_loader<Resource> Loader>
		inline std::shared_ptr<Resource> Cache<Resource, Loader>::create(const std::string& key, std::string_view file)
		{
			auto resource = m_loader.create(file);

			m_cache[key] = resource;
			return resource;
		}

		template<meta::not_memory Resource, is_loader<Resource> Loader>
		inline void Cache<Resource, Loader>::create_from_json(std::string_view json_file)
		{
			m_cache = m_loader.create_from_json(json_file);
		}

		template<meta::not_memory Resource, is_loader<Resource> Loader>
		inline void Cache<Resource, Loader>::clear()
		{
			m_cache.clear();
		}

		template<meta::not_memory Resource, is_loader<Resource> Loader>
		inline bool Cache<Resource, Loader>::has(const std::string& key) noexcept
		{
			return m_cache.contains(key);
		}

		template<meta::not_memory Resource, is_loader<Resource> Loader>
		inline std::shared_ptr<Resource> Cache<Resource, Loader>::get(const std::string& key) noexcept
		{
			if (has(key))
			{
				return m_cache[key];
			}

			return nullptr;
		}

		template<meta::not_memory Resource, is_loader<Resource> Loader>
		inline bool Cache<Resource, Loader>::empty() const noexcept
		{
			return m_cache.empty();
		}

		template<meta::not_memory Resource, is_loader<Resource> Loader>
		inline Holder<Resource>& Cache<Resource, Loader>::cache() noexcept
		{
			return m_cache;
		}
	} // namespace resource
} // namespace galaxy

#endif