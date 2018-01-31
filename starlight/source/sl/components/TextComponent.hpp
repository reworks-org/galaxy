///
/// TextComponent.hpp
/// starlight
///
/// Created by reworks on 19/08/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_TEXTCOMPONENT_HPP_
#define STARLIGHT_TEXTCOMPONENT_HPP_

#include <allegro5/color.h>

#include "sol2/sol_forward.hpp"

typedef struct ALLEGRO_FONT ALLEGRO_FONT;

namespace sl
{
	class TextComponent final
	{
	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data. Text parameter is max of 1024 characters. Font parameter is max of 1024 characters (in lua file).
		///
		TextComponent(const sol::table& table);

		///
		/// Destructor.
		///
		~TextComponent() = default;

		///
		/// Default move assignment overload.
		///
		TextComponent& operator=(const TextComponent&);

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		TextComponent() = delete;

	public:
		std::string m_id;

	private:
		std::string m_text;
		ALLEGRO_FONT* m_font;
		ALLEGRO_COLOR m_colour;
	};
}

#endif