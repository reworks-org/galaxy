///
/// UITheme.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_UITHEME_HPP_
#define CELESTIAL_UITHEME_HPP_

#include <vector>
#include <unordered_map>

#include <qs/text/Font.hpp>
#include <protostar/math/Rect.hpp>
#include <protostar/system/Colour.hpp>
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
		UITheme() noexcept;

		///
		/// Argument constructor.
		///
		/// \param atlasSize Size of the atlas to use.
		/// \param textures List of textures to compose texture atlas.
		/// \param fonts Name of font to load.
		///
		UITheme(const int atlasSize, const std::vector<std::string>& textures, const std::vector<celestial::FontData>& fonts);

		///
		/// Destructor.
		///
		~UITheme() noexcept;

		///
		/// Create the UITheme.
		///
		/// \param textures List of textures to compose texture atlas.
		/// \param fonts Name of font to load.
		///
		void create(const std::vector<std::string>& textures, const std::vector<celestial::FontData>& fonts);
		
		///
		/// Get a font using a key.
		///
		/// \param key The key is the "stem" of the file path. I.e. the filename without the extension or path.
		///
		qs::Font* getFont(const std::string& key);

	private:
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