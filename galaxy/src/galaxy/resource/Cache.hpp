///
/// Cache.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_CACHE_HPP_
#define GALAXY_RESOURCE_CACHE_HPP_

#include "galaxy/error/Log.hpp"
#include "galaxy/fs/Serializable.hpp"
#include "galaxy/resource/Loader.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Stores a cache of resources in order to make effective use of memory.
		/// resources can be a texture, sound, script, shader, etc...
		///
		/// \tparam Resource A resource can be anything that is not a pointer or a reference.
		/// \tparam ResourceLoader Loader to use when creating resources.
		///
		template<meta::not_memory Resource, is_loader<Resource> ResourceLoader>
		class Cache final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Cache() noexcept;

			///
			/// Destructor.
			///
			virtual ~Cache();

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
			/// Load a set of resources recursively from a root folder.
			///
			/// \param folder Folder to recursively search for files.
			///
			void create_from_folder(std::string_view folder);

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

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to be serialized.
			///
			[[nodiscard]] nlohmann::json serialize() override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			void deserialize(const nlohmann::json& json) override;

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
			ResourceLoader m_loader;

			///
			/// Contiguous resource array.
			///
			Holder<Resource> m_cache;
		};

		template<meta::not_memory Resource, is_loader<Resource> ResourceLoader>
		inline Cache<Resource, ResourceLoader>::Cache() noexcept
			: m_loader {}
		{
		}

		template<meta::not_memory Resource, is_loader<Resource> ResourceLoader>
		inline Cache<Resource, ResourceLoader>::~Cache()
		{
			clear();
		}

		template<meta::not_memory Resource, is_loader<Resource> ResourceLoader>
		inline std::shared_ptr<Resource> Cache<Resource, ResourceLoader>::create(const std::string& key, std::string_view file)
		{
			auto resource = m_loader.create(file);

			m_cache[key] = resource;
			return resource;
		}

		template<meta::not_memory Resource, is_loader<Resource> ResourceLoader>
		inline void Cache<Resource, ResourceLoader>::create_from_folder(std::string_view folder)
		{
			m_cache = std::move(m_loader.create_from_folder(folder));
		}

		template<meta::not_memory Resource, is_loader<Resource> ResourceLoader>
		inline void Cache<Resource, ResourceLoader>::clear()
		{
			m_cache.clear();
		}

		template<meta::not_memory Resource, is_loader<Resource> ResourceLoader>
		inline bool Cache<Resource, ResourceLoader>::has(const std::string& key) noexcept
		{
			return m_cache.contains(key);
		}

		template<meta::not_memory Resource, is_loader<Resource> ResourceLoader>
		inline std::shared_ptr<Resource> Cache<Resource, ResourceLoader>::get(const std::string& key) noexcept
		{
			if (has(key))
			{
				return m_cache[key];
			}

			return nullptr;
		}

		template<meta::not_memory Resource, is_loader<Resource> ResourceLoader>
		inline bool Cache<Resource, ResourceLoader>::empty() const noexcept
		{
			return m_cache.empty();
		}

		template<meta::not_memory Resource, is_loader<Resource> ResourceLoader>
		inline Holder<Resource>& Cache<Resource, ResourceLoader>::cache() noexcept
		{
			return m_cache;
		}

		template<meta::not_memory Resource, is_loader<Resource> ResourceLoader>
		inline nlohmann::json Cache<Resource, ResourceLoader>::serialize()
		{
			return m_loader.internal_serialize(m_cache);
		}

		template<meta::not_memory Resource, is_loader<Resource> ResourceLoader>
		inline void Cache<Resource, ResourceLoader>::deserialize(const nlohmann::json& json)
		{
			m_cache = m_loader.internal_deserialize(json);
		}
	} // namespace resource
} // namespace galaxy

#endif