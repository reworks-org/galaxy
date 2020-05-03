///
/// UITheme.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_UITHEME_HPP_
#define CELESTIAL_UITHEME_HPP_

#include <unordered_map>

#include <qs/text/Font.hpp>
#include <qs/core/Window.hpp>
#include <qs/renderer/Renderer.hpp>
#include <qs/graphics/TextureAtlas.hpp>

#include "celestial/Types.hpp"

///
/// Core namespace.
///
namespace celestial
{
	///
	/// Defines how your widgets appear for a UI.
	///
	class UITheme final
	{
	public:
		///
		/// Constructor.
		///
		/// \param atlasSize Size of the atlas to use.
		/// \param window Pointer to window.
		/// \param renderer Pointer to renderer.
		///
		UITheme(const int atlasSize, qs::Window* window, qs::Renderer* renderer) noexcept;

		///
		/// Destructor.
		///
		~UITheme() noexcept;

		///
		/// Create the UITheme.
		///
		/// \param shader Shader to use when drawing to the texture atlas.
		/// \param textures List of textures to compose texture atlas.
		/// \param fonts Name of font to load.
		///
		void create(qs::Shader& shader, const std::vector<std::string>& textures, const std::vector<celestial::FontData>& fonts) noexcept;
		
		///
		/// Get a font using a key.
		///
		/// \param key The key is the "stem" of the file path. I.e. the filename without the extension or path.
		///
		qs::Font* getFont(const std::string& key) noexcept;

		///
		/// Get pointer to associated window.
		///
		qs::Window* getWindow() const noexcept;

		///
		/// Get pointer to associated renderer.
		///
		qs::Renderer* getRenderer() const noexcept;

		///
		/// Get pointer to the TextureAtlas.
		///
		/// \return Pointer to the texture atlas.
		///
		qs::TextureAtlas* getAtlas() noexcept;

	private:
		///
		/// Default constructor.
		///
		UITheme() = delete;

	private:
		///
		/// Pointer to window.
		///
		qs::Window* m_window;

		///
		/// Pointer to renderer.
		///
		qs::Renderer* m_renderer;

		///
		/// Texture atlas containing all widget textures.
		///
		qs::TextureAtlas m_atlas;

		///
		/// Map of fonts stored for UI.
		///
		std::unordered_map<std::string, qs::Font> m_fonts;
	};
}

#endif