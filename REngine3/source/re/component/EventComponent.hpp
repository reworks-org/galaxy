//
//  EventComponent.hpp
//  REngine3
//
//  Created by reworks on 22/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_EVENTCOMPONENT_HPP_
#define RENGINE3_EVENTCOMPONENT_HPP_

#include "re/types/Event.hpp"
#include "re/types/Component.hpp"

namespace re
{
	class EventComponent : public Component
	{
		friend class EventSystem;
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
		void Init(sol::table& table) override;

		/*
		* IMPORTS: EventType and function pointer to function to call.
		* EXPORTS: none
		* PURPOSE: Submit a function to be called when an event is triggered.
		*/
		void SubmitOnEvent(EventType type, std::function<void(void)> func);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Check if the entity is subscribed to a particular event.
		*/
		bool IsSubscribed(EventType type);

	private:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Process event.
		*/
		void OnEvent(EventType type);

	private:
		std::unordered_map<EventType, std::vector<std::function<void(void)>>> m_events;
	};
}

#endif