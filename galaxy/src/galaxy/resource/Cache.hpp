///
/// Cache.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_CACHE_HPP_
#define GALAXY_RESOURCE_CACHE_HPP_

#include <robin_hood.h>

#include "galaxy/algorithm/FNV1a.hpp"
#include "galaxy/meta/Concepts.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Cache for resources such as audio, fonts, etc.
		///
		/// \tparam Resource Type of resource to manage.
		/// \tparam Loader Loader to use when loading the resource.
		/// \tparam needs_gl Does the loader need a opengl builder function.
		///
		template<meta::not_memory Resource, typename Loader, bool needs_gl>
		// requires meta::is_loader<Loader, Resource, needs_gl>
		class Cache final
		{
			using CacheType = robin_hood::unordered_node_map<std::uint64_t, std::shared_ptr<Resource>>;

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
			/// \tparam Args Variable arg types.
			///
			/// \param id Key to store resource with.
			/// \param args Arguments to pass to loader's operator().
			///
			template<typename... Args>
			inline void load(const std::string& id, Args&&... args)
			{
				m_keys.push_back(id);
				m_cache.emplace(hash(id), m_loader(std::forward<Args>(args)...));
			}

			///
			/// Load resources from a directory.
			///
			/// \param dir Folder to load from within VFS.
			///
			inline void load_folder(const std::string& dir)
			{
				clear();

				auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();
				for (const auto& file : fs.list_directory(dir))
				{
					const auto path = std::filesystem::path(file);
					const auto name = path.stem().string();

					load(name, path.string());
				}
			}

			///
			/// \brief Builds any OpenGL data.
			///
			/// Does nothing if this class doesn't require it.
			///
			inline void build()
			{
				if constexpr (needs_gl)
				{
					m_loader.build(m_cache);
				}
			}

			///
			/// Retrieve a resource.
			///
			/// \param id Key to use to access resource.
			///
			/// \return Returns a shared pointer to the resource.
			///
			[[nodiscard]] inline std::shared_ptr<Resource> get(std::string_view id)
			{
				const auto hashed = hash(id);

				if (m_cache.contains(hashed))
				{
					return m_cache[hashed];
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
			[[nodiscard]] inline bool has(std::string_view id)
			{
				return m_cache.contains(hash(id));
			}

			///
			/// Does the cache have any resources.
			///
			/// \return True if empty, false otherwise.
			///
			[[nodiscard]] inline bool empty() const
			{
				return size() == 0;
			}

			///
			/// Get amount of resources cached.
			///
			[[nodiscard]] inline std::size_t size() const
			{
				return m_cache.size();
			}

			///
			/// Get entire resource cache.
			///
			/// \return Const reference to the resource cache.
			///
			[[nodiscard]] inline const CacheType& cache() const
			{
				return m_cache;
			}

			///
			/// Get a list of keys in the cache.
			///
			/// \return A vector of char pointers.
			///
			[[nodiscard]] inline const meta::vector<std::string>& keys()
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
				const auto hash = algorithm::fnv1a_64(str.data());
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