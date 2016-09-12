//
//  UITextInput.hpp
//  ECSREngine
//
//  Created by reworks on 18/01/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef ECSRENGINE_SRC_TEXTINPUT_HPP_
#define ECSRENGINE_SRC_TEXTINPUT_HPP_

#include "re/graphics/ui/UIComponent.hpp"
#include "re/internal/SDLSmartRender.hpp"

namespace re
{
	class UITextInput : public UIComponent
	{
	public:

	   /**
	    * Constructor to create text input.
		* @param x The x position of the input area.
		* @param y The y position of the input area.
		* @param bw The width of the input area.
		* @param bh The height of the input area.
		* @param font The font to display in.
		* @param fontcol The colour of the font.
		* @param boxcol The colour of the input box.
		*/
		UITextInput(int x, int y, int bw, int bh, const std::string& font, SDL_Color fontcol, SDL_Color boxcolour);

		/**
		* Constructor to create text input.
		* @param x The x position of the input area.
		* @param y The y position of the input area.
		* @param bw The width of the input area.
		* @param bh The height of the input area.
		* @param font The font to display in.
		* @param fontcol The colour of the font.
		* @param boximage The texture for the input box.
		*/
		UITextInput(SDL_Rect dimensions, const std::string& font, SDL_Color fontcol, const std::string& boximage);

		/// Update the component. Called automatically.
		void Update() override;

		/// Render the component. Called automatically.
		void Render() override;

		/// Set the offset relative to the panel. Called automatically.
		void SetOffset(int x, int y) override;

		/// Parse SDL_Event structure.
		void HandleEvent(SDL_Event& e, int vx = 0, int vy = 0) override;

		/// Get the inputted text
		std::string GetText();

		/// Move the font point of origin around to line up with the box.
		void OffsetFont(int x, int y);

	private:
		int m_BoxWidth;
		int m_BoxHeight;
		bool m_DrawText = false;
		int m_MouseX;
		int m_MouseY;
		bool m_Inside;
		bool m_DrawCursor = true;
		int counter = 0;

		std::string m_Font;
		std::string m_InputText = "";
		std::string m_FinalText = "";

		SDL_Rect m_Dest;
		SDL_SmartTexture m_Text;

		SDL_Rect m_BoxDest;
		SDL_SmartTexture m_Box;
	};
}

#endif