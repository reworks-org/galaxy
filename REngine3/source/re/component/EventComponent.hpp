//
//  EventComponent.hpp
//  REngine3
//
//  Created by reworks on 22/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_EVENTCOMPONENT_HPP_
#define RENGINE3_EVENTCOMPONENT_HPP_

#include "re/component/Component.hpp"

namespace re
{
	typedef int EventType;

	class EventComponent : public Component
	{
		friend class EventSystem;
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: none yet.
		*/
		EventComponent();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up.
		*/
		~EventComponent() override;

		/*
		* IMPORTS: EventType and function pointer to function to call.
		* EXPORTS: none
		* PURPOSE: Submit a function to be called when an event is triggered.
		*/
		void SubmitOnEvent(EventType type, std::function<void(void)> func);

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


// CREATE A WAY TO "ACTIVATE" AND "DEACTIVATE" ENTITYS IN THE WORLD!!!!!