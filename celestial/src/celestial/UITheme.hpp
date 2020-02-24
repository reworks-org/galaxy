///
/// UITheme.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_UITHEME_HPP_
#define CELESTIAL_UITHEME_HPP_

#include <unordered_map>

#include <protostar/shapes/Rect.hpp>
#include <protostar/graphics/Colour.hpp>

#include "celestial/interface/Font.hpp"
#include "celestial/interface/Texture.hpp"
#include "celestial/interface/ResourceLoader.hpp"

///
/// Core namespace.
///
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
		UITheme() noexcept;

		///
		/// Destructor.
		///
		~UITheme() noexcept;

		///
		/// \brief Set the resource loader.
		///
		/// You need to call this function as soon as possible.
		///
		/// \param loader Polymorphic pointer to resource loader.
		///
		void setResourceLoader(celestial::interface::ResourceLoader* loader) noexcept;

		///
		/// Loads the master texture for the theme.
		///
		/// \param masterTexture Set the texture containing all the UI widget textures.
		///
		/// \return True if successful. False if resource loader not valid.
		///
		bool setMasterTexture(const std::string& texture) noexcept;

		///
		/// Sets the font used by the theme.
		///
		/// \param font Name of font to load.
		///
		/// \return True if successful. False if resource loader not valid.
		///
		bool setFont(const std::string& font) noexcept;

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
		void setColour(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a) noexcept;

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
		/// \return celestial::Texture of a sub-bitmap. Nullptr if resource loader is invalid.
		///
		celestial::interface::TexturePtr extractWidgetTexture(const std::string& id) noexcept;

		///
		/// Get theme font.
		///
		/// \return Font object.
		///
		celestial::interface::Font* getFont() const noexcept;

		///
		/// Get theme font, cast to a specific font type.
		///
		/// \return Font cast to FontType.
		///
		template<typename FontType>
		FontType* getFont() noexcept;

		///
		/// Get theme text colour.
		///
		/// \return Colour object.
		///
		const protostar::Colour& getColour() const noexcept;

		///
		/// Gets a reference to the resource loader.
		///
		celestial::interface::ResourceLoader* getLoader() const noexcept;

	private:
		///
		/// Colour of the text for UI.
		///
		protostar::Colour m_colour;

		///
		/// Loads resources.
		///
		celestial::interface::ResourceLoader* m_resourceLoader;

		///
		/// Font used for text in UI.
		///
		celestial::interface::FontPtr m_font;

		///
		/// Master texture containing textures for all the widgets.
		///
		celestial::interface::TexturePtr m_master;

		///
		/// Regions on the master texture for each widget.
		///
		std::unordered_map<std::string, protostar::Rect<int>> m_widgetRegions;
	};

	template<typename FontType>
	inline FontType* UITheme::getFont() noexcept
	{
		return dynamic_cast<FontType*>(m_font.get());
	}
}

#endif