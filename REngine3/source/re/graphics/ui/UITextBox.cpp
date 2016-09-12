//
//  UITextBox.cpp
//  ECSREngine
//
//  Created by reworks on 09/01/2015.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <iostream>

#include <SDL2/SDL_image.h>

#include "re/graphics/Window.hpp"
#include "re/platform/vfs/VFS.hpp"
#include "re/internal/ServiceLocator.hpp"

#include "UITextBox.hpp"

namespace re
{
	UITextBox::UITextBox(int x, int y, const std::string& image)
	{
		m_XPos = x;
		m_YPos = y;

		SDL_SmartSurface surf { IMG_Load_RW(Locator::Get<VFS>()->GetRWops(image), 1) };
		if (!surf) {
			LOG_ERROR(IMG_GetError());
		}	

		m_Dest.w = surf->w;
		m_Dest.h = surf->h;

		m_BoxBackground = SDL_CreateSmartTextureFromSurface(Locator::Get<Window>()->GetRenderer(), surf);
		if (!m_BoxBackground) {
			LOG_ERROR(SDL_GetError());
		}
	}

	UITextBox::UITextBox(int x, int y, int w, int h, SDL_Color col)
	{
		SDL_SmartSurface surf { SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0) };
		SDL_FillRect(surf.get(), nullptr, SDL_MapRGB(surf->format, col.r, col.g, col.b));

		m_Dest.w = surf->w;
		m_Dest.h = surf->h;

		m_BoxBackground = SDL_CreateSmartTextureFromSurface(Locator::Get<Window>()->GetRenderer(), surf);

		SDL_SetTextureBlendMode(m_BoxBackground.get(), SDL_BLENDMODE_BLEND);
		SDL_SetTextureAlphaMod(m_BoxBackground.get(), col.a);
	}

	UITextBox::~UITextBox()
	{
		m_Messages.clear();
	}

	void UITextBox::LoadMessages(const std::string& luaFile)
	{
		// Remove old messages
		m_Messages.clear();

		// Load new ones

	}

	void UITextBox::SetOffset(int x, int y)
	{
		m_OffsetXPos = x;
		m_OffsetYPos = y;

		m_Dest.x = m_XPos + m_OffsetXPos;
		m_Dest.y = m_YPos + m_OffsetYPos;
	}

	void UITextBox::HandleEvent(SDL_Event& e, int vx, int vy)
	{

	}

	void UITextBox::Update()
	{
		
	}

	void UITextBox::Render()
	{
		SDL_RenderCopy(Locator::Get<Window>()->GetRenderer(), m_BoxBackground.get(), nullptr, &m_Dest);
	}
}