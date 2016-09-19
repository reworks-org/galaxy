//
//  UIButton.cpp
//  REngine3
//
//  Created by reworks on 13/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
///

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "re/utils/Log.hpp"
#include "re/graphics/Window.hpp"
#include "re/internal/ServiceLocator.hpp"
#include "re/platform/vfs/VFS.hpp"
#include "re/graphics/font/FontManager.hpp"

#include "UIButton.hpp"

namespace re
{
	UIButton::UIButton(int x, int y, const std::string& btnsheet, SDL_Rect spriteclips[3])
	{
		// Set variables.
		m_XPos = x;
		m_YPos = y;

		m_ButtonState = BUTTON_MOUSE_OUT;

		m_BtnClips[0] = spriteclips[0];
		m_BtnClips[1] = spriteclips[1];
		m_BtnClips[2] = spriteclips[2];

		// Create the surface and check for errors.
		SDL_SmartSurface surf { IMG_Load_RW(Locator::Get<VFS>()->GetRWops(btnsheet), 1) };
		if (!surf) {
			LOG_ERROR(IMG_GetError());
		}

		// Create texture and check if valid.
		m_Button = SDL_CreateSmartTextureFromSurface(Locator::Get<Window>()->GetRenderer(), surf);
		if (!m_Button) {
			LOG_ERROR(SDL_GetError());
		}

		// Set dimensions to current state.
		m_Width = m_BtnClips[m_ButtonState].w;
		m_Height = m_BtnClips[m_ButtonState].h;
	}

	UIButton::UIButton(int x, int y, const std::string& text, const std::string& font, SDL_Color mout, SDL_Color mover, SDL_Color mdownup, bool hasDropShadow, int dsoffset)
	{
		m_XPos = x;
		m_YPos = y;
		m_Font = font;
		m_FontColours[0] = mout;
		m_FontColours[1] = mover;
		m_FontColours[2] = mdownup;

		m_ButtonState = BUTTON_MOUSE_OUT;

		m_IsText = true;

		m_HasDropShadow = hasDropShadow;
		m_DropShadowOffset = dsoffset;

		// Create surfaces and each text, along with the drop shadow for each button state.
		SDL_SmartSurface ds { TTF_RenderText_Blended(Locator::Get<FontManager>()->Get(font)->GetTTF().get(), text.c_str(), { 0, 0, 0 }) };
		m_TextButtonDropShadow = SDL_CreateSmartTextureFromSurface(Locator::Get<Window>()->GetRenderer(), ds);

		SDL_SmartSurface surf1 { TTF_RenderText_Blended(Locator::Get<FontManager>()->Get(font)->GetTTF().get(), text.c_str(), mout) };
		m_TextButtons[0] = SDL_CreateSmartTextureFromSurface(Locator::Get<Window>()->GetRenderer(), surf1);

		SDL_SmartSurface surf2 { TTF_RenderText_Blended(Locator::Get<FontManager>()->Get(font)->GetTTF().get(), text.c_str(), mover) };
		m_TextButtons[1] = SDL_CreateSmartTextureFromSurface(Locator::Get<Window>()->GetRenderer(), surf2);

		SDL_SmartSurface surf3 { TTF_RenderText_Blended(Locator::Get<FontManager>()->Get(font)->GetTTF().get(), text.c_str(), mdownup) };
		m_TextButtons[2] = SDL_CreateSmartTextureFromSurface(Locator::Get<Window>()->GetRenderer(), surf3);

		// Check everything to make sure its valid.
		for (int i = 0; i < 3; ++i) {
			if (!m_TextButtons[i] || !ds) {
				LOG_ERROR(TTF_GetError());
			}
		}
		
		m_Width = surf1->w;
		m_Height = surf1->h;
	}

	void UIButton::SetOffset(int x, int y)
	{
		// Set offsets.
		m_OffsetXPos = x;
		m_OffsetYPos = y;

		m_Dest.x = m_XPos + m_OffsetXPos;
		m_Dest.y = m_YPos + m_OffsetYPos;

		// Set positions.
		if (m_IsText) {
			SDL_QueryTexture(m_TextButtons[0].get(), nullptr, nullptr, &m_Dest.w, &m_Dest.h);
			// Set dimensions.
			if (m_HasDropShadow) {
				m_Destd.x = m_XPos + m_OffsetXPos + m_DropShadowOffset;
				m_Destd.y = m_YPos + m_OffsetYPos + m_DropShadowOffset;

				SDL_QueryTexture(m_TextButtonDropShadow.get(), nullptr, nullptr, &m_Destd.w, &m_Destd.h);
			}
		}
	}

	void UIButton::HandleEvent(SDL_Event& e, int vx, int vy)
	{
		// Check for a mouse event.
		if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
		{
			// Reset positions.
			int x = 0, y = 0;
			SDL_GetMouseState(&x, &y);
			m_Dest.x = m_XPos + m_OffsetXPos;
			m_Dest.y = m_YPos + m_OffsetYPos;

			// Is the click inside the button.
			bool inside = true;

			if (x < (m_XPos + m_OffsetXPos + vx)) { //mouse is left
				inside = false;
			} else if (x >(m_XPos + m_OffsetXPos + vx) + m_Width) { //mouse is right
				inside = false;
			} else if (y < (m_YPos + m_OffsetYPos + vy)) { //mouse is above
				inside = false;
			} else if (y >(m_YPos + m_OffsetYPos + vy) + m_Height) { //mouse is below
				inside = false;
			}

			if (!inside) {
				// Default state.
				m_ButtonState = BUTTON_MOUSE_OUT;
				m_IsClicked = false;
			} else {

				switch (e.type)
				{
				case SDL_MOUSEMOTION:
					// Set if hovering.
					m_ButtonState = BUTTON_MOUSE_OVER;
					m_IsClicked = false;
					break;

				case SDL_MOUSEBUTTONDOWN:
					// On click.
					m_Dest.x = m_XPos + m_OffsetXPos + m_DropShadowOffset;
					m_Dest.y = m_YPos + m_OffsetYPos + m_DropShadowOffset;
					if (m_HasSoundEffect) { Mix_PlayChannel(-1, m_SoundEffect.get(), 0); }
					m_ButtonState = BUTTON_MOUSE_DOWN;
					m_IsClicked = true;
					break;

				case SDL_MOUSEBUTTONUP:
					// On release.
					m_ButtonState = BUTTON_MOUSE_UP;
					m_IsClicked = false;
					m_ButtonState = BUTTON_MOUSE_OUT;
					break;
				}
			}
		}
	}

	void UIButton::Update()
	{
		// If its not text, update dimensions.
		if (!m_IsText) {
			m_Clip = m_BtnClips[m_ButtonState];

			m_Width = m_Clip.w;
			m_Height = m_Clip.h;

			m_Dest.w = m_Clip.w;
			m_Dest.h = m_Clip.h;
		}
	}

	void UIButton::Render()
	{
		// Draw depending on type.
		if (!m_IsText) {
			SDL_RenderCopy(Locator::Get<Window>()->GetRenderer(), m_Button.get(), &m_Clip, &m_Dest);
		} else {
			if (m_HasDropShadow) {
				SDL_RenderCopy(Locator::Get<Window>()->GetRenderer(), m_TextButtonDropShadow.get(), nullptr, &m_Destd);
			}

			SDL_RenderCopy(Locator::Get<Window>()->GetRenderer(), m_TextButtons[m_ButtonState].get(), nullptr, &m_Dest);
		}
	}

	bool UIButton::IsClicked()
	{
		return m_IsClicked;
	}

	void UIButton::UnClick()
	{
		m_IsClicked = false;
	}

	void UIButton::AddSoundEffect(SDL_RWops* rw, int volume)
	{
		m_SoundEffect = Mix_SmartLoadWAV_RW(rw);
		if (!m_SoundEffect) {
			LOG_ERROR(Mix_GetError());

		} else {
			m_HasSoundEffect = true;
			Mix_VolumeChunk(m_SoundEffect.get(), volume);
		}
	}

	void UIButton::ChangeButtonText(const std::string& text)
	{
		// Create surfaces and each text, along with the drop shadow for each button state.
		SDL_SmartSurface ds{ TTF_RenderText_Blended(Locator::Get<FontManager>()->Get(m_Font)->GetTTF().get(), text.c_str(), { 0, 0, 0 }) };
		m_TextButtonDropShadow = SDL_CreateSmartTextureFromSurface(Locator::Get<Window>()->GetRenderer(), ds);

		SDL_SmartSurface surf1{ TTF_RenderText_Blended(Locator::Get<FontManager>()->Get(m_Font)->GetTTF().get(), text.c_str(), m_FontColours[0]) };
		m_TextButtons[0] = SDL_CreateSmartTextureFromSurface(Locator::Get<Window>()->GetRenderer(), surf1);

		SDL_SmartSurface surf2{ TTF_RenderText_Blended(Locator::Get<FontManager>()->Get(m_Font)->GetTTF().get(), text.c_str(), m_FontColours[1]) };
		m_TextButtons[1] = SDL_CreateSmartTextureFromSurface(Locator::Get<Window>()->GetRenderer(), surf2);

		SDL_SmartSurface surf3{ TTF_RenderText_Blended(Locator::Get<FontManager>()->Get(m_Font)->GetTTF().get(), text.c_str(), m_FontColours[2]) };
		m_TextButtons[2] = SDL_CreateSmartTextureFromSurface(Locator::Get<Window>()->GetRenderer(), surf3);

		m_Width = surf1->w;
		m_Height = surf1->h;
	}
}