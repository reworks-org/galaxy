//
//  UIButton.hpp
//  REngine3
//
//  Created by reworks on 13/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_UIBUTTON_HPP_
#define RENGINE3_UIBUTTON_HPP_

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
		UIButton(const std::string& script);
		~UIButton() override;

		/// Check for an event.
		void Event() override;

		// Overrides.
		void SetOffset(int x, int y) override;
		void Update() override;
	
		/// Is the button clicked?
		bool IsClicked();

		/// Manually unclick a button.
		void UnClick();

		///Volume Range: 0 - 128
		void AddSoundEffect();
		
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