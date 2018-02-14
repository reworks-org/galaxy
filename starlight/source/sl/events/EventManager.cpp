///
/// EventManager.cpp
/// starlight
///
/// Created by reworks on 15/01/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "EventManager.hpp"

namespace sl
{
	EventManager::EventManager()
	{
		m_queue = al_create_event_queue();
		al_init_user_event_source(&m_userSource);
		al_register_event_source(m_queue, &m_userSource);
	}

	EventManager::~EventManager()
	{
		al_destroy_event_queue(m_queue);
	}

	void EventManager::emitEvent(EventType type, intptr_t data1, intptr_t data2, intptr_t data3, intptr_t data4, void(*dtor)(ALLEGRO_USER_EVENT *))
	{
		ALLEGRO_EVENT ev;

		ev.type = type;
		ev.user.data1 = data1;
		ev.user.data2 = data2;
		ev.user.data3 = data3;
		ev.user.data4 = data4;

		al_emit_user_event(&m_userSource, &ev, dtor);
	}
}