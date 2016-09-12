//
//  UITextBox.cpp
//  ECSREngine
//
//  Created by reworks on 09/01/2015.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_clipboard.h>

#include "re/utils/Config.hpp"
#include "re/graphics/Window.hpp"
#include "re/platform/vfs/VFS.hpp"
#include "re/internal/ServiceLocator.hpp"
#include "re/graphics/font/FontManager.hpp"

#include "UITextInput.hpp"

namespace re
{
	UITextInput::UITextInput(int x, int y, int bw, int bh, const std::string& font, SDL_Color fontcol, SDL_Color boxcolour)
	{
		m_XPos = x;
		m_YPos = y;
		
		m_BoxHeight = bh;
		m_BoxWidth = bw;
		m_Font = font;

		SetColour(fontcol);
		
		if (m_InputText != "") {
			// Create an sdl2 texture of the text.
			SDL_SmartSurface surf { TTF_RenderText_Blended(Locator::Get<FontManager>()->Get(m_Font)->GetTTF().get(), m_InputText.c_str(), m_Colour) };

			m_Dest.w = surf->w;
			m_Dest.h = surf->h;

			m_Text = SDL_CreateSmartTextureFromSurface(Locator::Get<Window>()->GetRenderer(), surf);
		}

		// Create some surfaces and prepare them.
		SDL_SmartSurface surf2 { SDL_CreateRGBSurface(0, m_BoxWidth, m_BoxHeight, 32, 0, 0, 0, 0) };
		

		// Fill the surfaces in with pixels.
		SDL_FillRect(surf2.get(), nullptr, SDL_MapRGB(surf2->format, boxcolour.r, boxcolour.g, boxcolour.b));
		
		m_BoxDest.x = m_XPos;
		m_BoxDest.y = m_YPos;
		m_BoxDest.w = surf2->w;
		m_BoxDest.h = surf2->h;

		// Create the textures.
		m_Box = SDL_CreateSmartTextureFromSurface(Locator::Get<Window>()->GetRenderer(), surf2);

		// Set alpha blending on the textures.
		SDL_SetTextureBlendMode(m_Box.get(), SDL_BLENDMODE_BLEND);
		SDL_SetTextureAlphaMod(m_Box.get(), boxcolour.a);
	}

	UITextInput::UITextInput(SDL_Rect dimensions, const std::string& font, SDL_Color fontcol, const std::string& boximage)
	{
		m_XPos = dimensions.x;
		m_YPos = dimensions.y;

		m_BoxHeight = dimensions.h;
		m_BoxWidth = dimensions.w;
		m_Font = font;

		SetColour(fontcol);

		if (m_InputText != "") {
			// Create an sdl2 texture of the text.
			SDL_SmartSurface surf{ TTF_RenderText_Blended(Locator::Get<FontManager>()->Get(m_Font)->GetTTF().get(), m_InputText.c_str(), m_Colour) };

			m_Dest.w = surf->w;
			m_Dest.h = surf->h;

			m_Text = SDL_CreateSmartTextureFromSurface(Locator::Get<Window>()->GetRenderer(), surf);
		}

		// Create some surfaces and prepare them.
		SDL_SmartSurface surf2 { IMG_Load_RW(Locator::Get<VFS>()->GetRWops(boximage), 1) };

		m_BoxDest.x = m_XPos;
		m_BoxDest.y = m_YPos;
		m_BoxDest.w = surf2->w;
		m_BoxDest.h = surf2->h;

		// Create the textures.
		m_Box = SDL_CreateSmartTextureFromSurface(Locator::Get<Window>()->GetRenderer(), surf2);
	}

	void UITextInput::Update()
	{
		if (m_DrawText) {
			if (m_InputText != "") {
				// Create an sdl2 texture of the text.
				SDL_SmartSurface surf{ TTF_RenderText_Blended(Locator::Get<FontManager>()->Get(m_Font)->GetTTF().get(), m_InputText.c_str(), m_Colour) };
				
				m_Dest.w = surf->w;
				m_Dest.h = surf->h;
				
				m_Text = SDL_CreateSmartTextureFromSurface(Locator::Get<Window>()->GetRenderer(), surf);

			} else {
				// Create an sdl2 texture of the space.
				SDL_SmartSurface surf{ TTF_RenderText_Blended(Locator::Get<FontManager>()->Get(m_Font)->GetTTF().get(), " ", m_Colour) };

				m_Dest.w = surf->w;
				m_Dest.h = surf->h;

				m_Text = SDL_CreateSmartTextureFromSurface(Locator::Get<Window>()->GetRenderer(), surf);
			}
		}

		++counter;
		if (counter >= (Locator::Get<ConfigReader>()->Lookup<int>("targetUPS") / 1.1) - 1 && counter <= (Locator::Get<ConfigReader>()->Lookup<int>("targetUPS") / 1.1) + 1)
		{
			if (m_DrawCursor == true)
			{
				m_DrawCursor = false;
			}
			else if (m_DrawCursor == false)
			{
				m_DrawCursor = true;
			}

			counter = 0;
		}
	}

	void UITextInput::Render()
	{
		SDL_RenderCopy(Locator::Get<Window>()->GetRenderer(), m_Box.get(), nullptr, &m_BoxDest);
		SDL_RenderCopy(Locator::Get<Window>()->GetRenderer(), m_Text.get(), nullptr, &m_Dest);
		SDL_SetRenderDrawColor(Locator::Get<Window>()->GetRenderer(), m_Colour.r, m_Colour.g, m_Colour.b, m_Colour.a);

		if (m_DrawText && m_DrawCursor) {
			
			SDL_RenderDrawLine(Locator::Get<Window>()->GetRenderer(), m_BoxDest.x + m_Dest.w, m_BoxDest.y, m_BoxDest.x + m_Dest.w, m_BoxDest.y + m_BoxHeight);
		}
		SDL_SetRenderDrawColor(Locator::Get<Window>()->GetRenderer(), 96, 96, 96, 0);
	}

	void UITextInput::SetOffset(int x, int y)
	{
		m_OffsetXPos = x;
		m_OffsetYPos = y;

		// Set destination coords.
		m_Dest.x = m_XPos + m_OffsetXPos;
		m_Dest.y = m_YPos + m_OffsetYPos;

		m_BoxDest.x = m_Dest.x;
		m_BoxDest.y = m_Dest.y;
	}

	void UITextInput::HandleEvent(SDL_Event& e, int vx, int vy)
	{
		if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
		{
			SDL_GetMouseState(&m_MouseX, &m_MouseY);

			m_Dest.x = m_XPos + m_OffsetXPos;
			m_Dest.y = m_YPos + m_OffsetYPos;

			// Is the click inside the button.	
			m_Inside = true;

			if (m_MouseX < (m_XPos + m_OffsetXPos + vx)) { //mouse is left
				m_Inside = false;
			}
			else if (m_MouseX >(m_XPos + m_OffsetXPos + vx) + m_BoxWidth) { //mouse is right
				m_Inside = false;
			}
			else if (m_MouseY < (m_YPos + m_OffsetYPos + vy)) { //mouse is above
				m_Inside = false;
			}
			else if (m_MouseY >(m_YPos + m_OffsetYPos + vy) + m_BoxHeight) { //mouse is below
				m_Inside = false;
			}
		}

		if (m_Inside)
		{
			m_DrawText = true;
			switch (e.type)
			{
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_BACKSPACE && m_InputText.length() > 0) {
					m_InputText.pop_back();
				} else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
					SDL_SetClipboardText(m_InputText.c_str());
				} else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
					m_InputText = SDL_GetClipboardText();
				} else if (e.key.keysym.sym == SDLK_RETURN) {
					m_FinalText = m_InputText;
					m_Inside = false;
				}
				break;
			}

			SDL_StartTextInput();

			while (SDL_PollEvent(&e) != 0)
			{
				// Poll for events and do string manipulations.
				if (e.type == SDL_TEXTINPUT) {
					if (!((e.text.text[0] == 'c' || e.text.text[0] == 'C') && (e.text.text[0] == 'v' || e.text.text[0] == 'V') && SDL_GetModState() & KMOD_CTRL)) {
						m_InputText += e.text.text;
						m_DrawText = true;
					}
				}
			}

			SDL_StopTextInput();
		}
		else
		{
			m_DrawText = false;
		}
	}

	std::string UITextInput::GetText()
	{
		return m_FinalText;
	}

	void UITextInput::OffsetFont(int x, int y)
	{
		m_Dest.x = m_XPos + m_OffsetXPos + x;
		m_Dest.y = m_YPos + m_OffsetYPos + y;
	}
}