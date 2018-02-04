///
/// EventManager.hpp
/// starlight
///
/// Created by reworks on 15/01/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_EVENTMANAGER_HPP_
#define STARLIGHT_EVENTMANAGER_HPP_

#include <allegro5/events.h>

#include "sl/events/EventTypes.hpp"
#include "sl/types/ResourceCache.hpp"
#include "sl/types/ServiceLocator.hpp"

namespace sl
{
	class EventManager : public ServiceLocator<EventManager>, public ResourceCache<ALLEGRO_EVENT>
	{
	public:
		///
		/// \brief Constructor.
		///
		/// Initialises event queue.
		///
		EventManager();

		///
		/// Cleans up events.
		///
		~EventManager();

		///
		/// \brief Add a user event to the system.
		///
		/// WARNING: WILL NOT DELETE MEMORY THAT IS USED IN data1-data4. YOU MUST HANDLE MEMORY CLEANUP YOURSELF.
		///
		/// \param id The string ID of the event to create.
		/// \param type An EventType (int). Use one from EventTypes data structure.
		/// \param data1 First data package included with event. E.g. float a; then pass &a.
		/// \param data2 Second data package included with event.
		/// \param data3 Third data package included with event.
		/// \param data4 Fourth data package included with event.
		///
		void createUserEvent(const std::string& id, EventType type, intptr_t data1 = NULL, intptr_t data2 = NULL, intptr_t data3 = NULL, intptr_t data4 = NULL);

		///
		/// Emit a user event.
		///
		/// \param id ID of event to emit.
		///
		void emitUserEvent(const std::string& id);

		///
		/// Clean up resources.
		///
		void clean() override;

	public:
		ALLEGRO_EVENT_QUEUE* m_queue;

	private:
		ALLEGRO_EVENT_SOURCE m_userSource;
	};
}

#endif