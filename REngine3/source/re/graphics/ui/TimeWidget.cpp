//
//  TimeWidget.cpp
//  REngine3
//
//  Created by reworks on 10/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/utility/Time.hpp"

#include "TimeWidget.hpp"

namespace re
{
	TimeWidget::TimeWidget()
		: tgui::Label()
	{
	}

	void TimeWidget::update(sf::Time elapsedTime)
	{
		Widget::update(elapsedTime);

		// When double-clicking, the second click has to come within 500 milliseconds
		if (m_animationTimeElapsed >= sf::milliseconds(500))
		{
			m_animationTimeElapsed = {};
			m_possibleDoubleClick = false;
		}

		std::string temp = Time::GetCurrentTimeAndDate();

		temp.erase(temp.length() - 8);

		if (std::stoi(temp.substr(11, 2)) >= 12) {
			temp.append(" PM");
		}
		else {
			temp.append(" AM");
		}

		temp = temp.substr(11, 8);

		setText(temp);
	}
}