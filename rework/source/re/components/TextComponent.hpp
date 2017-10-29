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
#include "re/types/Renderable.hpp"

namespace re
{
	class TextComponent : public Renderable
	{
	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data. Text parameter is max of 1024 characters. Font parameter is max of 1024 characters.
		///
		TextComponent(entityx::Entity& e, sol::table& table);

		///
		/// Destructor.
		///
		~TextComponent() override;
		
		///
		/// Calls imgui debug functions. Don't call this, done for you by debugmanager.
		///
		void debug();

		///
		/// Draw text.
		///
		void render() override;

	public:
		std::string m_text;
		ALLEGRO_COLOR m_colour;
		ALLEGRO_FONT* m_font;
		float m_offsetX;
		float m_offsetY;
	};
}

#endif