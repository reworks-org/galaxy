///
/// Theme.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_THEME_HPP_
#define CELESTIAL_THEME_HPP_

#include <protostar/utility/UniqueID.hpp>
#include <protostar/system/ResourceCache.hpp>

///
/// Core namespace.
///
namespace celestial
{
	///
	/// Unique ID generator for UI Theme.
	///
	using ThemeCacheUID = protostar::UniqueID<struct _ThemeCacheUID>;

	///
	/// Interface that stores protostar::ResourceCache's for widgets to access.
	///
	class Theme final
	{
	public:
		///
		/// Default constructor.
		///
		Theme() noexcept = default;
		
		///
		/// Destructor.
		///
		~Theme() noexcept;

		///
		/// \brief Add a cache to the theme.
		///
		/// This stores a pointer, does not create/destroy/manage memory. You need to do that seperately.
		///
		/// \param cache Pointer to resource cache to store.
		///
		template<typename Cache>
		void store(Cache* cache) noexcept;

		///
		/// Get a resource from a cache.
		///
		/// \param id ID of resource in cache to get.
		///
		/// \return Pointer to the resource. Or nullptr.
		///
		template<typename Cache>
		decltype(auto) get(const unsigned int id) noexcept;

	private:
		///
		/// Pointers to resource caches.
		///
		std::vector<protostar::ResCacheBase*> m_caches;
	};

	template<typename Cache>
	inline void Theme::store(Cache* cache) noexcept
	{
		constexpr auto type = ThemeCacheUID::get<Cache>();
		if (type >= m_caches.size())
		{
			m_caches.resize(type + 1, nullptr);
		}

		if (m_caches[type] == nullptr)
		{
			m_caches[type] = dynamic_cast<protostar::ResCacheBase*>(cache);
		}
		else
		{
			PL_LOG(PL_WARNING, "Tried to add duplicate Cache.");
		}
	}

	template<typename Cache>
	inline decltype(auto) Theme::get(const unsigned int id) noexcept
	{
		constexpr auto type = ThemeCacheUID::get<Cache>();
		if (type >= m_caches.size())
		{
			PL_LOG(PL_ERROR, "Cache type does not exist: " + std::to_string(id));
			return nullptr;
		}
		else
		{
			auto* cache = dynamic_cast<Cache*>(m_caches[type]);
			return cache->get(id);
		}
	}
}

#endif