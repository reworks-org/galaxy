//
//  TextComponent.hpp
//  rework
//
//  Created by reworks on 19/08/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_TEXTCOMPONENT_HPP_
#define REWORK_TEXTCOMPONENT_HPP_

#include "sol2/sol.hpp"

namespace re
{
	class TextComponent
	{
	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
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

	public:
		std::string m_text;
		unsigned int m_layer;
		ALLEGRO_COLOR m_colour;
		ALLEGRO_FONT* m_font;
		float m_x;
		float m_y;
	};
}

#endif