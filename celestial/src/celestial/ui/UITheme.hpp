///
/// UITheme.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_UITHEME_HPP_
#define CELESTIAL_UITHEME_HPP_

#include <unordered_map>

#include "celestial/compat/Font.hpp"
#include "protostar/graphics/Colour.hpp"
#include "protostar/graphics/Rect.hpp"
#include "celestial/compat/Texture.hpp"

#include "celestial/compat/ResourceLoader.hpp"

namespace celestial
{
	///
	/// Class that defines how your widgets appear for a UI.
	///
	class UITheme final
	{
	public:
		///
		/// Constructor.
		///
		/// \param resourceLoader Loads resources for theme.
		///
		UITheme(ResourceLoader* resourceLoader);

		///
		/// Destructor.
		///
		~UITheme() noexcept;

		///
		/// Loads the master texture for the theme.
		///
		/// \param masterTexture Set the texture containing all the UI widget textures.
		///
		void setMasterTexture(const std::string& texture);

		///
		/// Sets the font used by the theme.
		///
		/// \param font Name of font to load.
		///
		void setFont(const std::string& font);

		///
		/// \brief Sets the colour used by the theme. 
		///
		/// Template type is the child type that inherits from celestial's compat colour class. Must be able to take r,g,b,a constructor.
		///
		/// \param r Red.
		/// \param g Green.
		/// \param b Blue.
		/// \param a Alpha.
		///
		template<typename ColourClass>
		void setColour(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a);

		///
		/// Define a specific texture region inside the master texture for a widget.
		///
		/// \param id ID of the texture region for that widget. I.e. "ButtonOn".
		/// \param dim Dimensions of that texture on the master texture.
		///
		void defineWidgetTexture(const std::string& id, const protostar::Rect<int>& dim) noexcept;

		///
		/// Gets a sub-bitmap of a widget texture region from the master texture.
		///
		/// \param id ID of the texture region in the widget regions to use as a texture.
		///
		/// \return celestial::Texture of a sub-bitmap.
		///
		TexturePtr extractWidgetTexture(const std::string& id) noexcept;

		///
		/// Get theme font.
		///
		/// \return Font object.
		///
		const Font* font() const noexcept;

		///
		/// Get theme text colour.
		///
		/// \return Colour object.
		///
		const protostar::colour colour() const noexcept;

		///
		/// Gets a reference to the resource loader.
		///
		ResourceLoader* loader() const noexcept;

	private:
		///
		/// Default Constructor.
		///
		UITheme() = delete;

	private:
		///
		/// Loads resources.
		///
		ResourceLoader* m_resourceLoader;

		///
		/// Font used for text in UI.
		///
		FontPtr m_font;

		///
		/// Colour of the text for UI.
		///
		ColourPtr m_colour;

		///
		/// Master texture containing textures for all the widgets.
		///
		TexturePtr m_master;

		///
		/// Regions on the master texture for each widget.
		///
		std::unordered_map<std::string, protostar::Rect<int>> m_widgetRegions;
	};

	template<typename ColourClass>
	inline void UITheme::setColour(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a)
	{
		m_colour = std::make_unique<ColourClass>(r, g, b, a);
	}

}

#endif