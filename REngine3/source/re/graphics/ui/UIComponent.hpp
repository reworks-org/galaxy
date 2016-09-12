//
//  UIComponent.hpp
//  REngine3
//
//  Created by reworks on 1/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_UICOMPONENT_HPP_
#define RENGINE3_UICOMPONENT_HPP_

#include <string>

#include <SFML/Graphics/Drawable.hpp>

#include "re/services/vfs/sfmlphysfs.hpp"

namespace re
{
	class UIComponent : public sf::Drawable
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up component.
		*/
		virtual inline ~UIComponent() {}

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Handle events.
		*/
		virtual void Event() = 0;
		
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Update component.
		*/
		virtual void Update() = 0;

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Set the offset of the component from the panel.
		*/
		virtual void SetOffset(int x, int y);
		
		/*
		* IMPORTS: none
		* EXPORTS: boolean, true if it is deleted.
		* PURPOSE: Is the component deleted?
		*/
		bool IsDeleted();

	protected:
		int m_xpos;
		int m_ypos;

		int m_offsetX;
		int m_offsetY;

		int m_finalPosX;
		int m_finalPosY;

		bool m_deleted = false;
	};
}

#endif