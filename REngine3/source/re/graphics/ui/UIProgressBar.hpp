//
//  UIProgressBar.hpp
//  ECSREngine
//
//  Created by reworks on 15/10/2015.
//  Copyright © 2016 reworks. All rights reserved.
//

#ifndef ECSRENGINE_SRC_UIPROGRESSBAR_HPP_
#define ECSRENGINE_SRC_UIPROGRESSBAR_HPP_

#include "re/graphics/ui/UIComponent.hpp"
#include "re/internal/SDLSmartRender.hpp"

namespace re
{
	class UIProgressBar : public UIComponent
	{
	public:
	    /** Constructor
		* @param dim An SDL_Rect containing the dimensions of the bar. This includes x pos, y pos, width and height.
		* @param backcol Background colour of the bar.
		* @param forecol Foreground colour of the bar.
		*/
		UIProgressBar(SDL_Rect dim, SDL_Color backcol, SDL_Color forecol);

		/**
		* @param x XPos of image.
		* @param y YPos of image.
		* @param imageback a string containing the filename of the image to load in the .zip file(s) for the background.
		* @param imagefore a string containing the filename of the image to load in the .zip file(s) for the foreground.
		*/
		//UIProgressBar(int x, int y, const char* imageback, const char* imagefore);

		~UIProgressBar() override;

		/// Set the progress of the bar. value provided is divided by 100.0. The range of the bar is 0.0 to 1.0.
		void SetProgress(double progress);

		/// Get the progress of the bar.
		double GetProgress();

		/// Sets offsets for calculations. Called automatically.
		void SetOffset(int x, int y) override;
		
		/// Update the bar.
		void Update() override;

		/// Render the bar.
		void Render() override;

		/// Handle Events
		inline void HandleEvent(SDL_Event& e, int vx = 0, int vy = 0) override {}

	private:
		int m_Width;
		int m_Height;

		/// 0.0 - 1.0 (0% to 100%).
		double m_Progress;

		SDL_SmartTexture m_BarFore;
		SDL_SmartTexture m_BarBack;
		SDL_Color m_ForegroundColour;
		SDL_Rect m_Dest;
		SDL_Rect m_DestFore;
	};
}

#endif