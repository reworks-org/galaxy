//
//  EventComponent.hpp
//  REngine3
//
//  Created by reworks on 22/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_EVENTCOMPONENT_HPP_
#define RENGINE3_EVENTCOMPONENT_HPP_

#include "re/types/Events.hpp"
#include "re/types/Component.hpp"

namespace re
{
	class EventComponent : public Component
	{
		friend class EventSystem;
        typedef std::unordered_map<EventType, std::vector<std::function<void(void)>>> EventStorage;
        
    public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Default Constructor.
		*/
		EventComponent();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up.
		*/
		~EventComponent() override;

		/*
		* IMPORTS: sol::table from lua script containing component data.
		* EXPORTS: none
		* PURPOSE: Set up the component.
		*/
		void init(sol::table& table) override;

		/*
		* IMPORTS: none
		* EXPORTS: Whether or not to save the changed table data.
		* PURPOSE: debug component, change data, etc.
		*/
		bool debugFunction(sol::state& state) override;

		/*
		* IMPORTS: EventType and function pointer to function to call.
		* EXPORTS: none
		* PURPOSE: Submit a function to be called when an event is triggered.
		*/
		void submitOnEvent(EventType type, std::function<void(void)> func);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Check if the entity is subscribed to a particular event.
		*/
		bool isSubscribed(EventType type);

	private:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Process event.
		*/
		void onEvent(EventType type);

	private:
		EventStorage m_events;
	};
}

#endif
