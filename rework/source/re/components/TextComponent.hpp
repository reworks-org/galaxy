///
///  TextComponent.hpp
///  rework
///
///  Created by reworks on 19/08/2016.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef REWORK_TEXTCOMPONENT_HPP_
#define REWORK_TEXTCOMPONENT_HPP_

#include <allegro5/color.h>

#include "sol2/sol_forward.hpp"

typedef struct ALLEGRO_FONT ALLEGRO_FONT;

namespace re
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
		/// Move Constructor.
		///
		TextComponent(TextComponent&&) = default;

		///
		/// Destructor.
		///
		~TextComponent() override;
		
		///
		/// Calls imgui debug functions. Don't call this, done for you by debugmanager.
		///
		void debug();

	public:
		float m_offsetX;
		float m_offsetY;
		std::string m_text;
		ALLEGRO_FONT* m_font;
		ALLEGRO_COLOR m_colour;

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		TextComponent() = delete;

		///
		/// Copy Constructor.
		/// Deleted.
		///
		TextComponent(const TextComponent&) = delete;
	};
}

#endif