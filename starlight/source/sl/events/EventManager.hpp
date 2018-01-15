///
/// EventManager.hpp
/// starlight
///
///  Created by reworks on 15/01/2018.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_EVENTMANAGER_HPP_
#define STARLIGHT_EVENTMANAGER_HPP_

#include "sl/types/ServiceLocator.hpp"

namespace sl
{
	class EventManager : public ServiceLocator<EventManager>
	{
	};
}

#endif