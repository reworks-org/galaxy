//
//  UITextBox.hpp
//  ECSREngine
//
//  Created by reworks on 09/01/2015.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef ECSRENGINE_SRC_TEXTBOX_HPP_
#define ECSRENGINE_SRC_TEXTBOX_HPP_

#include <vector>

#include "re/internal/SDLSmartFont.hpp"
#include "re/internal/SDLSmartRender.hpp"
#include "re/graphics/ui/UIComponent.hpp"

namespace re
{
	class UITextBox : public UIComponent
	{
	public:
		// XPOS, YPOS, IMAGE NAME
		UITextBox(int x, int y, const std::string& image);
		UITextBox(int x, int y, int w, int h, SDL_Color col);

		~UITextBox() override;

		void HandleEvent(SDL_Event& e, int vx = 0, int vy = 0) override;
		void Update() override;
		void Render() override;

		void SetOffset(int x, int y) override;

		void LoadMessages(const std::string& luaFile);

	private:
		std::vector<std::string> m_Messages;

		SDL_Rect m_Dest;
		SDL_SmartTexture m_BoxBackground;
		SDL_SmartTexture m_Text;
	};
}

#endif

// If text texture width is larger than the box + buffer, move down the origin point by the height of the text + buffer.
// once all text is displayed, on a keypress, delete object.
// create object when needed
// use two strings? one for drawing one for holding a message? maybe in a vector?
// http://code.tutsplus.com/tutorials/make-an-rpg-style-text-system-for-your-next-game--active-3678
// https://www.google.com.au/search?q=jrpg+textbox+example+implementation&ie=utf-8&oe=utf-8&client=firefox-b-ab&gfe_rd=cr&ei=Q498V5PhFq7M8gfAw5foDA