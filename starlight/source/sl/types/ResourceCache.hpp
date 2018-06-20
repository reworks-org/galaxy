///
/// ResourceCache.hpp
/// starlight
///
/// Created by reworks on 17/12/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_RESOURCECACHE_HPP_
#define STARLIGHT_RESOURCECACHE_HPP_

#include <type_traits>
#include <string_view>
#include <unordered_map>

#include "sl/utils/Utils.hpp"

namespace sl
{
	///
	/// Stores a cache of resources in order to make effective use of memory.
	/// Resources can be i.e. ALLEGRO_FONT* or Sound, etc.
	///
	template<typename Resource>
	class ResourceCache
	{
	public:
		///
		/// Destructor.
		///
		virtual ~ResourceCache() = default;

		///
		/// Retrieve a resource.
		///
		/// \param name The name of the resource to retrieve.
		///
		/// \return Returns a pointer to the resource.
		///
		inline typename utils::ReturnReferenceIfFalse<std::is_pointer<Resource>::value, Resource>::type get(const std::string& id)
		{
			if (m_resourceMap.find(id) != m_resourceMap.end())
			{
				return m_resourceMap[id];
			}
			else
			{
				LOG_S(FATAL) << "Attempted to access a non-existant resource: " << id;
				return m_resourceMap.at(id); // at() is used here because it throws an exception if it does not exist.
			}
		}

		///
		/// Clean up resources.
		///
		virtual void clean() = 0;

	protected:
		///
		/// Default constructor.
		///
		ResourceCache() = default;

		///
		/// Copy Constructor.
		/// Deleted.
		///
		ResourceCache(const ResourceCache&) = delete;

		///
		/// Move Constructor.
		/// Deleted.
		///
		ResourceCache(ResourceCache&&) = delete;

	protected:
		///
		/// The hashmap containing the resources.
		/// A string_view is used because it is easier to use than integer identifiers and it is more efficient than std::string.
		///
		std::unordered_map<std::string, Resource> m_resourceMap;
	};
}

#endif