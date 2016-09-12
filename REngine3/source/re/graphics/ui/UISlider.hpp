//
//  UISlider.hpp
//  ECSREngine
//
//  Created by reworks on 23/12/2015.
//  Copyright © 2016 reworks. All rights reserved.
//

#ifndef ECSRENGINE_SRC_UISLIDER_HPP_
#define ECSRENGINE_SRC_UISLIDER_HPP_

#include "re/graphics/ui/UIComponent.hpp"
#include "re/internal/SDLSmartRender.hpp"

namespace re
{
	class UISlider : public UIComponent
	{
	public:
		/**
		 * @param dim SDL_Rect for the dimensions. x pos, y pos, width and height.
		 * @param backcol background colour of slider.
		 * @param forecol foreground colour of slider.
		 * @param font keyword for font from fontmanager.
		 * @param fx x-pos position of the percentage text.
		 * @param fy y-pos position of the percentage text.
		*/
		UISlider(SDL_Rect dim, SDL_Color backcol, SDL_Color forecol, const char* font, SDL_Color fontcol, int fx, int fy);

		/**
		 * @param x x-pos of slider relative to panel
		 * @param y y-pos of slider relative to panel
		 * @param imagefore the foreground image
		 * @param imageback the background image
		 * @param font keyword for font from fontmanager.
		 * @param fx x-pos position of the percentage text.
		 * @param fy y-pos position of the percentage text.
		*/
		//UISlider(int x, int y, const char* imagefore, const char* imageback, const char* font, SDL_Color fontcol, int fx, int fy);

		// Overrides from UIComponents.
		void SetOffset(int x, int y) override;
		void Update() override;
		void Render() override;

		/// Create a box supplying width, height and colour.
		void CreateSliderBar(int w, int h, SDL_Color col);

		/// Check for an event.
		void HandleEvent(SDL_Event& e, int vx = 0, int vy = 0) override;
		
		/// Manually set the progress value.
		void SetProgress(double progress);

		/// Get the progress value.
		double GetProgress();

	private:
		int m_Width;
		int m_Height;

		SDL_Color m_FontCol;
		const char* m_Font;
		SDL_Rect m_FontDest;

		/// 0.0 - 1.0 (0% to 100%).
		double m_Progress;

		SDL_SmartTexture m_BarFore;
		SDL_SmartTexture m_BarBack;
		SDL_SmartTexture m_Percentage;
		SDL_SmartTexture m_SliderBar;

		SDL_Color m_ForegroundColour;
		SDL_Rect m_Dest;
		SDL_Rect m_DestFore;
		SDL_Rect m_SliderBarDest;
		
	};
}

#endif