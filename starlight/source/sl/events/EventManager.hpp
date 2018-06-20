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

namespace sl
{
	///
	/// Used to create and emit user events in allegro.
	///
	class EventManager final
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
		/// \brief Emit a user event.
		///
		/// WARNING: WILL NOT DELETE MEMORY THAT IS USED IN data1-data4. YOU MUST HANDLE MEMORY CLEANUP YOURSELF.
		///
		/// \param type An EventType (int). Use one from EventTypes data structure.
		/// \param data1 First data package included with event. E.g. float a; then pass &a.
		/// \param data2 Second data package included with event.
		/// \param data3 Third data package included with event.
		/// \param data4 Fourth data package included with event.
		/// \param dtor A function to clean up any memory allocated with data1-data4.
		///
		void emitEvent(EventType type, intptr_t data1 = NULL, intptr_t data2 = NULL, intptr_t data3 = NULL, intptr_t data4 = NULL, void(*dtor)(ALLEGRO_USER_EVENT *) = nullptr) noexcept;

	public:
		///
		/// The user queue of events.
		///
		ALLEGRO_EVENT_QUEUE* m_queue;

	private:
		///
		/// The source to emit user events from.
		///
		ALLEGRO_EVENT_SOURCE m_userSource;
	};
}

#endif