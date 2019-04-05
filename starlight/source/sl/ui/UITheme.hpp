///
/// UITheme.hpp
/// starlight
///
/// Created by reworks on 15/09/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_UITHEME_HPP_
#define STARLIGHT_UITHEME_HPP_

#include <string>
#include <unordered_map>

#include <allegro5/color.h>

#include "sl/math/Rect.hpp"
#include "sl/libs/sol2/forward.hpp"

typedef struct ALLEGRO_FONT ALLEGRO_FONT;
typedef struct ALLEGRO_COLOR ALLEGRO_COLOR;
typedef struct ALLEGRO_BITMAP ALLEGRO_BITMAP;

namespace sl
{
	///
	/// Class that defines how your widgets appear for a UI.
	///
	class UITheme
	{
	public:
		///
		/// Constructor.
		///
		/// \param font Set the default font for the UI from the FontBook.
		/// \param colour Set the default text colour for the UI.
		/// \param masterTexture Set the texture containing all the UI widget textures.
		///
		UITheme(const std::string& font, const ALLEGRO_COLOR colour, const std::string& masterTexture);
		
		///
		/// Lua Constructor.
		///
		/// \param table Lua table containing info.
		///
		explicit UITheme(const sol::table& table);

		///
		/// Destructor.
		///
		~UITheme();

		///
		/// Define a specific texture region inside the master texture for a widget.
		///
		/// \param id ID of the texture region for that widget. I.e. "ButtonOn".
		/// \param dim Dimensions of that texture on the master texture.
		///
		void defineWidgetTexture(const std::string& id, const sl::Rect<int>& dim);

		///
		/// Gets a sub-bitmap of a widget texture region from the master texture.
		///
		/// \param id ID of the texture region in the widget regions to use as a texture.
		///
		/// \return ALLEGRO_BITMAP* of a sub-bitmap. You need to free this bitmap. Shares memory with parent.
		///			See: https://liballeg.org/a5docs/trunk/graphics.html#al_create_sub_bitmap
		///
		ALLEGRO_BITMAP* widgetTexture(const std::string& id);

		///
		/// Get theme font.
		///
		/// \return ALLEGRO_FONT pointer.
		///
		ALLEGRO_FONT* font() const;

		///
		/// Get theme text colour.
		///
		/// \return ALLEGRO_Color object.
		///
		ALLEGRO_COLOR colour() const;

	private:
		///
		/// Default Constructor.
		///
		UITheme() = delete;

	private:
		///
		/// Font used for text in UI.
		///
		ALLEGRO_FONT* m_font;

		///
		/// Colour of the text for UI.
		///
		ALLEGRO_COLOR m_colour;

		///
		/// Master texture containing textures for all the widgets.
		///
		ALLEGRO_BITMAP* m_masterTexture;

		///
		/// Regions on the master texture for each widget.
		///
		std::unordered_map<std::string, sl::Rect<int>> m_widgetRegions;
	};

}

#endif