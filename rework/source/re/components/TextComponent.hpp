//
//  TextComponent.hpp
//  rework
//
//  Created by reworks on 19/08/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_TEXTCOMPONENT_HPP_
#define REWORK_TEXTCOMPONENT_HPP_

#include <allegro5/allegro_font.h>

#include "sol2/sol.hpp"

namespace re
{
	class TextComponent
	{
	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data. Text parameter is max of 1024 characters. Font parameter is max of 1024 characters.
		///
		TextComponent(sol::table& table);

		///
		/// Destructor.
		///
		~TextComponent();

		///
		/// Draw text. Convience function.
		///
		void draw();

		///
		/// Calls imgui debug functions. Don't call this, done for you by debugmanager.
		///
		void debug();

	public:
		std::string m_text;
		ALLEGRO_COLOR m_colour;
		ALLEGRO_FONT* m_font;
		float m_x;
		float m_y;
		unsigned int m_layer;
	};
}

#endif