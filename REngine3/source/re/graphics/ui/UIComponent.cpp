//
//  UIComponent.cpp
//  REngine3
//
//  Created by reworks on 1/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "UIComponent.hpp"

namespace re
{
	void UIComponent::SetOffset(int x, int y)
	{
		m_offsetX = x;
		m_offsetY = y;

		m_finalPosX = m_xpos + m_offsetX;
		m_finalPosY = m_ypos + m_offsetY;
	}

	bool UIComponent::IsDeleted()
	{
		return m_deleted;
	}
}