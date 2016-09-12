//
//  UIButton.hpp
//  ECSREngine
//
//  Created by reworks on 15/10/2015.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef ECSRENGINE_SRC_UIBUTTON_HPP_
#define ECSRENGINE_SRC_UIBUTTON_HPP_

#include "re/internal/SDLSmartAudio.hpp"
#include "re/internal/SDLSmartRender.hpp"
#include "re/graphics/ui/UIComponent.hpp"

enum ButtonState
{
	BUTTON_MOUSE_OUT = 0,
	BUTTON_MOUSE_OVER = 1,
	BUTTON_MOUSE_DOWN = 2,
	BUTTON_MOUSE_UP = 2
};

namespace re
{
	class UIButton : public UIComponent
	{
	public:
		/**
		* Constructor for Text based buttons. Supports 3 states: Click, Hover, Out
		* @param x XPos relative to the panel.
		* @param y YPos relative to the panel.
		* @param btnsheet The spritesheet containing the buttons.
		* @param spriteclips[] The dimensions of each sprite on the sheet. Pass an array.
		*/
		UIButton(int x, int y, const std::string& btnsheet, SDL_Rect spriteclips[3]);

		/**
		* Constructor for Text based buttons. Supports 3 states: Click, Hover, Out
		* @param x XPos relative to the panel.
		* @param y YPos relative to the panel.
		* @param text Text to display.
		* @param font Key for the FontManager.
		* @param mout Colour when mouse is out.
		* @param mover Colour when mouse is hovering.
		* @param mdownup Colour when button is clicked.
		* @param hasDropShadow does it have a drop shadow?
		* @param dsoffet the offset of the drop shadow. default 0.
		*/
		UIButton(int x, int y, const std::string& text, const std::string& font, SDL_Color mout, SDL_Color mover, SDL_Color mdownup, bool hasDropShadow = false, int dsoffset = 0);

		/// Check for an event.
		void HandleEvent(SDL_Event& e, int vx = 0, int vy = 0) override;

		// Overrides.
		void SetOffset(int x, int y) override;
		void Update() override;
		void Render() override;

		/// Is the button clicked?
		bool IsClicked();

		/// Manually unclick a button.
		void UnClick();

		///Volume Range: 0 - 128
		void AddSoundEffect(SDL_RWops* rw, int volume);
		
		/// Change text.
		void ChangeButtonText(const std::string& text);

	private:
		Mix_SmartChunk m_SoundEffect;
		SDL_SmartTexture m_Button;
		SDL_SmartTexture m_TextButtonDropShadow;
		SDL_SmartTexture m_TextButtons[3];

		int m_Width;
		int m_Height;

		std::string m_Font;
		SDL_Color m_FontColours[3];

		bool m_IsText = false;
		bool m_IsClicked = false;
		bool m_HasSoundEffect = false;

		SDL_Rect m_BtnClips[3];
		SDL_Rect m_Dest;
		SDL_Rect m_Clip;

		bool m_HasDropShadow;
		int m_DropShadowOffset;
		SDL_Rect m_Destd;

		ButtonState m_ButtonState;
	};
}

#endif