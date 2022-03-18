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
		/// \tparam Loader Loader to use when creating resources.
		///
		template<meta::not_memory Resource, is_loader<Resource> Loader>
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
		inline void Cache<Resource, Loader>::create_from_folder(std::string_view folder)
		{
			m_cache = std::move(m_loader.create_from_folder(folder));
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

		template<meta::not_memory Resource, is_loader<Resource> Loader>
		inline nlohmann::json Cache<Resource, Loader>::serialize()
		{
			return m_loader.internal_serialize(m_cache);
		}

		template<meta::not_memory Resource, is_loader<Resource> Loader>
		inline void Cache<Resource, Loader>::deserialize(const nlohmann::json& json)
		{
			m_cache = m_loader.internal_deserialize(json);
		}
	} // namespace resource
} // namespace galaxy

#endif