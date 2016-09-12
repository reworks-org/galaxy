//
//  UISlider.cpp
//  ECSREngine
//
//  Created by reworks on 23/12/2015.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <SDL2/SDL_ttf.h>

#include "re/utils/Log.hpp"
#include "re/utils/Utils.hpp"
#include "re/graphics/Window.hpp"
#include "re/internal/ServiceLocator.hpp"
#include "re/graphics/font/FontManager.hpp"

#include "UISlider.hpp"

namespace re
{
	UISlider::UISlider(SDL_Rect dim, SDL_Color backcol, SDL_Color forecol, const char* font, SDL_Color fontcol, int fx, int fy)
	{
		m_XPos = dim.x;
		m_YPos = dim.y;
		m_Width = dim.w;
		m_Height = dim.h;
		m_ForegroundColour = forecol;
		m_Font = font;
		m_FontCol = fontcol;
		m_FontDest.x = fx;
		m_FontDest.y = fy;
		SetColour(backcol);

		// Create some surfaces and prepare them.
		SDL_SmartSurface surf { SDL_CreateRGBSurface(0, m_Width, m_Height, 32, 0, 0, 0, 0) };
		SDL_SmartSurface surf2 { SDL_CreateRGBSurface(0, m_Width, m_Height, 32, 0, 0, 0, 0) };

		// Fill the surfaces in with pixels.
		SDL_FillRect(surf.get(), nullptr, SDL_MapRGB(surf->format, m_Colour.r, m_Colour.g, m_Colour.b));
		SDL_FillRect(surf2.get(), nullptr, SDL_MapRGB(surf2->format, m_ForegroundColour.r, m_ForegroundColour.g, m_ForegroundColour.b));

		// Create the textures.
		m_BarBack = SDL_CreateSmartTextureFromSurface(Locator::Get<Window>()->GetRenderer(), surf);
		m_BarFore = SDL_CreateSmartTextureFromSurface(Locator::Get<Window>()->GetRenderer(), surf2);

		// Set alpha blending on the textures.
		SDL_SetTextureBlendMode(m_BarBack.get(), SDL_BLENDMODE_BLEND);
		SDL_SetTextureAlphaMod(m_BarBack.get(), m_Colour.a);

		SDL_SetTextureBlendMode(m_BarFore.get(), SDL_BLENDMODE_BLEND);
		SDL_SetTextureAlphaMod(m_BarFore.get(), m_ForegroundColour.a);

		SDL_SmartSurface fontsurf { TTF_RenderText_Blended(Locator::Get<FontManager>()->Get(m_Font)->GetTTF().get(), "0%", m_FontCol) };
		m_Percentage = SDL_CreateSmartTextureFromSurface(Locator::Get<Window>()->GetRenderer(), fontsurf);

		m_Dest.w = surf->w;
		m_Dest.h = surf->h;
		m_DestFore.w = surf2->w;
		m_DestFore.h = surf2->h;
	}

	void UISlider::CreateSliderBar(int w, int h, SDL_Color col)
	{
		m_SliderBarDest.w = w;
		m_SliderBarDest.h = h;

		SDL_SmartSurface surf { SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0) };
		SDL_FillRect(surf.get(), nullptr, SDL_MapRGB(surf->format, col.r, col.g, col.b));

		m_SliderBar = SDL_CreateSmartTextureFromSurface(Locator::Get<Window>()->GetRenderer(), surf);

		SDL_SetTextureBlendMode(m_SliderBar.get(), SDL_BLENDMODE_BLEND);
		SDL_SetTextureAlphaMod(m_SliderBar.get(), col.a);
	}

	void UISlider::SetProgress(double progress)
	{
		// Progress works in decimals from 0 - 1.
		// Make sure its not out of range.
		progress = progress / 100.0;
		if (progress < 0.0) {
			LOG_WARNING("Tried to set progress too low. Defaulting to 0");
			m_Progress = 0.0;

		} else if (progress > 1.0) {
			LOG_WARNING("Tried to set progress too high. Defaulting to 100");
			m_Progress = 1.0;

		} else {
			m_Progress = progress;
		}

		// Change the % label.
		double val = m_Progress * 100.0;
		std::string lbl = std::to_string((int)val) + "%";
		SDL_SmartSurface fontsurf{ TTF_RenderText_Blended(Locator::Get<FontManager>()->Get(m_Font)->GetTTF().get(), lbl.c_str(), m_FontCol) };
		m_Percentage = SDL_CreateSmartTextureFromSurface(Locator::Get<Window>()->GetRenderer(), fontsurf);
	}
	
	double UISlider::GetProgress()
	{
		return m_Progress;
	}

	void UISlider::SetOffset(int x, int y)
	{
		// Set offset and rendering coordinates for foreground and background textures.
		m_OffsetXPos = x;
		m_OffsetYPos = y;

		m_Dest.x = m_XPos + m_OffsetXPos;
		m_Dest.y = m_YPos + m_OffsetYPos;

		m_DestFore.x = m_XPos + m_OffsetXPos;
		m_DestFore.y = m_YPos + m_OffsetYPos;

		m_SliderBarDest.x = m_DestFore.w + (m_SliderBarDest.w / 2);
		m_SliderBarDest.y = m_DestFore.y - (m_SliderBarDest.h / 4);

		m_FontDest.x = m_FontDest.x + m_OffsetXPos;
		m_FontDest.y = m_FontDest.y + m_OffsetYPos;
	}

	void UISlider::HandleEvent(SDL_Event& e, int vx, int vy)
	{
		// Check for a mouse event.
		if ((e.type == SDL_MOUSEMOTION && e.button.button == SDL_BUTTON_LEFT) || e.type == SDL_MOUSEBUTTONDOWN)
		{
			// Reset positions.
			int x, y;
			SDL_GetMouseState(&x, &y);
			m_Dest.x = m_XPos + m_OffsetXPos;
			m_Dest.y = m_YPos + m_OffsetYPos;

			// Is the click inside the button.
			bool inside = true;

			// Calculate if the mouse is inside the slider. Buffer is included so there is room to move the mouse around.
			if (x < (m_XPos + m_OffsetXPos + vx - 4)) { // Mouse is left.
				inside = false;
			} else if (x >(m_XPos + m_OffsetXPos + vx) + m_Width + 4) { // Mouse is right.
				inside = false;
			} else if (y < (m_YPos + m_OffsetYPos + vy - 10)) { // Mouse is above.
				inside = false;
			} else if (y >(m_YPos + m_OffsetYPos + vy) + m_Height + 10) { // Mouse is below.
				inside = false;
			}

			if (inside) {
				// On click.
				int newx = 0;
				SDL_GetMouseState(&newx, nullptr);

				// Need to fix this formula, works, but not sure why (specifically the / 2.5 part).
				double progress = (newx - (m_Dest.x + vx)) / 2.5;
				SetProgress(progress);
			}
		}
	}

	void UISlider::Update()
	{
		// Change its position..
		m_DestFore.w = static_cast<int>(m_Progress * m_Width);
		m_SliderBarDest.x = m_DestFore.w + (m_SliderBarDest.w / 2);
		SDL_QueryTexture(m_Percentage.get(), nullptr, nullptr, &m_FontDest.w, &m_FontDest.h);
	}

	void UISlider::Render()
	{
		// Render textures.
		SDL_RenderCopy(Locator::Get<Window>()->GetRenderer(), m_BarBack.get(), nullptr, &m_Dest);
		SDL_RenderCopy(Locator::Get<Window>()->GetRenderer(), m_BarFore.get(), nullptr, &m_DestFore);
		SDL_RenderCopy(Locator::Get<Window>()->GetRenderer(), m_SliderBar.get(), nullptr, &m_SliderBarDest);
		SDL_RenderCopy(Locator::Get<Window>()->GetRenderer(), m_Percentage.get(), nullptr, &m_FontDest);
	}
}