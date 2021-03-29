///
/// ResourceCache.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RES_RESOURCECACHE_HPP_
#define GALAXY_RES_RESOURCECACHE_HPP_

#include <robin_hood.h>

#include "galaxy/error/Log.hpp"
#include "galaxy/fs/Serializable.hpp"
#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace res
	{
		///
		/// Shortcut.
		///
		template<meta::not_pointer_or_ref Holder>
		using ResourceHolder = robin_hood::unordered_flat_map<std::string, std::unique_ptr<Holder>>;

		///
		/// Container structure for storing resource cache objects in containers.
		///
		struct ResCacheBase
		{
			///
			/// Default constructor.
			///
			ResCacheBase() = default;

			///
			/// Virtual default destructor.
			///
			virtual ~ResCacheBase() = default;
		};

		///
		/// Stores a cache of resources in order to make effective use of memory.
		/// resources can be a texture, sound, script, shader, etc...
		///
		template<meta::not_pointer_or_ref Resource>
		class ResourceCache : public ResCacheBase, public fs::Serializable
		{
		public:
			///
			/// Get rid of ugly template syntax.
			///
			using Cache = ResourceHolder<Resource>;

			///
			/// Destructor.
			///
			virtual ~ResourceCache() = default;

			///
			/// Create a resource.
			///
			/// \param name Should be name of resource without path or extension.
			/// \param args Constructor arguments.
			///
			/// \return Const pointer to newly created resource.
			///
			template<typename... Args>
			[[maybe_unused]] Resource* const create(std::string_view name, Args&&... args);

			///
			/// Retrieve a resource.
			///
			/// \param handle The name of the file without path or extension.
			///
			/// \return Returns a pointer to the resource.
			///
			[[nodiscard]] Resource* const get(std::string_view handle);

			///
			/// Get entire resource cache.
			///
			/// \return Reference to the resource holders cache. An unordered_flat_map of unique_ptr's.
			///
			[[nodiscard]] Cache& cache() noexcept;

			///
			/// Clean up.
			///
			virtual void clear() noexcept = 0;

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to be serialized.
			///
			[[nodiscard]] virtual nlohmann::json serialize() = 0;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			virtual void deserialize(const nlohmann::json& json) = 0;

		protected:
			///
			/// Constructor.
			///
			inline ResourceCache() noexcept
			    : fs::Serializable {this}
			{
			}

		protected:
			///
			/// Contiguous resource array.
			///
			Cache m_resources;

		private:
			///
			/// Copy constructor.
			///
			ResourceCache(const ResourceCache&) = delete;

			///
			/// Move constructor.
			///
			ResourceCache(ResourceCache&&) = delete;

			///
			/// Copy assignment operator.
			///
			ResourceCache& operator=(const ResourceCache&) = delete;

			///
			/// Move assignment operator.
			///
			ResourceCache& operator=(ResourceCache&&) = delete;
		};

		template<meta::not_pointer_or_ref Resource>
		template<typename... Args>
		inline Resource* const ResourceCache<Resource>::create(std::string_view name, Args&&... args)
		{
			const auto str = static_cast<std::string>(name);
			if (!m_resources.contains(str))
			{
				m_resources[str] = std::make_unique<Resource>(args...);
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to insert duplicate resource.");
			}

			return m_resources[str].get();
		}

		template<meta::not_pointer_or_ref Resource>
		inline Resource* const ResourceCache<Resource>::get(std::string_view name)
		{
			const auto str = static_cast<std::string>(name);
			if (m_resources.contains(str))
			{
				return m_resources[str].get();
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Failed to find resource with name: " + str);
				return nullptr;
			}
		}

		template<meta::not_pointer_or_ref Resource>
		inline ResourceCache<Resource>::Cache& ResourceCache<Resource>::cache() noexcept
		{
			return m_resources;
		}
	} // namespace res
} // namespace galaxy

#endif