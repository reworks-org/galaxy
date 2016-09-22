//
//  EventSystem.hpp
//  REngine3
//
//  Created by reworks on 23/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_EVENTSYSTEM_HPP_
#define RENGINE3_EVENTSYSTEM_HPP_

#include "re/types/Event.hpp"
#include "re/types/System.hpp"

namespace re
{
	class EventSystem : public System
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Cleans up the events.
		*/
		~EventSystem() override;

		/*
		* IMPORTS: entity and event type.
		* EXPORTS: none
		* PURPOSE: Subscribe an entity to a specific event.
		*/
		void Subscribe(std::shared_ptr<Entity> e, EventType type);

		/*
		* IMPORTS: Event to dispatch
		* EXPORTS: none
		* PURPOSE: Dispatch an event to subscribed entitys.
		*/
		void Dispatch(EventType t);
	private:
		std::unordered_map<EventType, std::vector<std::shared_ptr<Entity>>> m_listenerList;
	};
}

#endif