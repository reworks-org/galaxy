///
/// Theme.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_THEME_HPP_
#define GALAXY_THEME_HPP_

#include <qs/sprite/SpriteBatch.hpp>
#include <qs/graphics/TextureAtlas.hpp>
#include <protostar/utility/UniqueID.hpp>
#include <protostar/system/ResourceCache.hpp>

///
/// Core namespace.
///
namespace galaxy
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
		/// Constructor.
		///
		Theme() noexcept;
		
		///
		/// Destructor.
		///
		~Theme() noexcept;

		///
		/// Set pointer to window.
		///
		/// \param window Non-owning pointer to window.
		///
		void setWindow(qs::Window* window) noexcept;

		///
		/// Set pointer to renderer.
		///
		/// \param renderer Non-owning pointer to renderer.
		///
		void setRenderer(qs::Renderer* renderer) noexcept;

		///
		/// Add a texture to the atlas.
		///
		/// \param texture File path to texture.
		///
		void addTextureToAtlas(const std::string& texture) noexcept;

		///
		/// Create texture atlas.
		///
		/// \param shader Shader to create atlas with.
		///
		void createTextureAtlas(qs::Shader& shader) noexcept;

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
		/// \param name Name of the resource to get. Filename without path or extension.
		///
		/// \return Pointer to the resource. Or nullptr.
		///
		template<typename Cache>
		decltype(auto) get(std::string_view name) noexcept;

		///
		/// Get pointer to texture atlas.
		///
		/// \return Pointer.
		///
		qs::TextureAtlas* getAtlas() noexcept;

		///
		/// Get pointer to sprite batch.
		///
		/// \return Pointer.
		///
		qs::SpriteBatch* getBatch() noexcept;

		///
		/// Get pointer to window.
		///
		/// \return Pointer.
		///
		qs::Window* getWindow() const noexcept;

		///
		/// Get pointer to renderer.
		///
		/// \return Pointer.
		///
		qs::Renderer* getRenderer() const noexcept;

	private:
		///
		/// Window pointer.
		///
		qs::Window* m_window;

		///
		/// Renderer pointer.
		///
		qs::Renderer* m_renderer;

		///
		/// Texture Atlas.
		///
		qs::TextureAtlas m_atlas;

		///
		/// SpriteBatch.
		///
		qs::SpriteBatch m_spriteBatch;

		///
		/// Pointers to resource caches.
		///
		std::vector<protostar::ResCacheBase*> m_caches;
	};

	template<typename Cache>
	inline void Theme::store(Cache* cache) noexcept
	{
		const auto type = ThemeCacheUID::get<Cache>();
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
	inline decltype(auto) Theme::get(std::string_view name) noexcept
	{
		const auto type = ThemeCacheUID::get<Cache>();
		if (type >= m_caches.size())
		{
			PL_LOG(PL_ERROR, "Cache type does not exist: " + std::string(name));
		}

		auto* cache = dynamic_cast<Cache*>(m_caches[type]);
		return cache->get(name);
	}
}

#endif