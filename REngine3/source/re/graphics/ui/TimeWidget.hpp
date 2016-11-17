//
//  TimeWidget.hpp
//  REngine3
//
//  Created by reworks on 10/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_TIMEWIDGET_HPP_
#define RENGINE3_TIMEWIDGET_HPP_

#include <TGUI/Widgets/Label.hpp>

namespace re
{
	class TimeWidget : public tgui::Label
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Default Constructor.
		*/
		TimeWidget();

	private:
		/*
		* IMPORTS: elapsed time
		* EXPORTS: none
		* PURPOSE: Update the widget
		*/
		virtual void update(sf::Time elapsedTime) override;
	};
}

#endif