///
/// EventManager.cpp
/// starlight
///
/// Created by reworks on 15/01/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <allegro5/error.h>
#include "sl/libs/loguru/loguru.hpp"

#include "EventManager.hpp"

namespace sl
{
	EventManager::EventManager()
	{
		// Create user queue and user event source so library can emit custom events to allegro.
		m_queue = al_create_event_queue();

		// Null over nullptr because allegro explictly returns NULL on error.
		if (m_queue != NULL)
		{
			al_init_user_event_source(&m_userSource);
			al_register_event_source(m_queue, &m_userSource);
		}
		else
		{
			LOG_S(FATAL) << "Failed to create a user event queue. al_errno: " << al_get_errno();
		}
	}

	EventManager::~EventManager()
	{
		// Clean up the user event queue.
		al_destroy_event_queue(m_queue);
	}

	void EventManager::emitEvent(EventType type, intptr_t data1, intptr_t data2, intptr_t data3, intptr_t data4, void(*dtor)(ALLEGRO_USER_EVENT *)) noexcept
	{
		// Set the apropriate data...
		ALLEGRO_EVENT ev;

		ev.type = type;
		ev.user.data1 = data1;
		ev.user.data2 = data2;
		ev.user.data3 = data3;
		ev.user.data4 = data4;

		// ...then emit the event.
		al_emit_user_event(&m_userSource, &ev, dtor);
	}
}