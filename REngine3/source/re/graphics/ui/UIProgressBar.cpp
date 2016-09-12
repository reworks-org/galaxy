//
//  UIProgressBar.cpp
//  ECSREngine
//
//  Created by reworks on 15/10/2015.
//  Copyright © 2016 reworks. All rights reserved.
//

#include "re/utils/Log.hpp"
#include "re/graphics/Window.hpp"
#include "re/internal/ServiceLocator.hpp"

#include "UIProgressBar.hpp"

namespace re
{
	UIProgressBar::UIProgressBar(SDL_Rect dim, SDL_Color backcol, SDL_Color forecol)
	{
		m_XPos = dim.x;
		m_YPos = dim.y;
		m_Width = dim.w;
		m_Height = dim.h;
		m_ForegroundColour = forecol;
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
	}

	UIProgressBar::~UIProgressBar()
	{
	}

	void UIProgressBar::SetProgress(double progress)
	{
		// Progress works in decimals from 0 - 1.
		// Make sure its not out of range.
		progress = progress / 100.0;
		if (progress < 0.0) {
			LOG_INFO("Tried to set progress too low. Defaulting to 0");
			m_Progress = 0.0;

		} else if (progress > 1.0) {
			LOG_INFO("Tried to set progress too high. Defaulting to 100");
			m_Progress = 1.0;

		} else {
			m_Progress = progress;
		}
	}

	double UIProgressBar::GetProgress()
	{
		return m_Progress;
	}

	void UIProgressBar::SetOffset(int x, int y)
	{
		// Set offset and rendering coordinates for foreground and background textures.
		m_OffsetXPos = x;
		m_OffsetYPos = y;

		m_Dest.x = m_XPos + m_OffsetXPos;
		m_Dest.y = m_YPos + m_OffsetYPos;

		m_DestFore.x = m_XPos + m_OffsetXPos;
		m_DestFore.y = m_YPos + m_OffsetYPos;

		SDL_QueryTexture(m_BarBack.get(), nullptr, nullptr, &m_Dest.w, &m_Dest.h);
		SDL_QueryTexture(m_BarFore.get(), nullptr, nullptr, &m_DestFore.w, &m_DestFore.h);
	}

	void UIProgressBar::Update()
	{
		// Change its position.
		m_DestFore.w = static_cast<int>(m_Progress * m_Width);
	}

	void UIProgressBar::Render()
	{
		// Render textures.
		SDL_RenderCopy(Locator::Get<Window>()->GetRenderer(), m_BarBack.get(), nullptr, &m_Dest);
		SDL_RenderCopy(Locator::Get<Window>()->GetRenderer(), m_BarFore.get(), nullptr, &m_DestFore);
	}
}