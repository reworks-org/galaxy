///
/// Dispatcher.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_DISPATCHER_DISPATCHER_HPP_
#define GALAXY_EVENTS_DISPATCHER_DISPATCHER_HPP_

#include <functional>
#include <vector>

#include "galaxy/error/Log.hpp"
#include "galaxy/events/dispatcher/Storage.hpp"

namespace galaxy
{
	namespace events
	{
		///
		/// This is the main class to dispatch events from.
		///
		class Dispatcher final
		{
		public:
			///
			/// Default constructor.
			///
			Dispatcher() noexcept = default;

			///
			/// Default destructor.
			///
			~Dispatcher();

			///
			/// \brief Registers a function to be called on the triggering of an event.
			///
			/// Event is the event to register.
			/// Receiver is the type of object.
			///
			/// \param reciever Object that has an on_event() function that takes a const Event&.
			///
			template<meta::is_class Event, meta::is_class Receiver>
			void subscribe(Receiver& receiver) requires meta::has_on_event_for<Receiver, Event>;

			///
			/// Triggers a single event.
			///
			/// \param args Arguments to construct event to trigger.
			///
			template<meta::is_class Event, typename... Args>
			void trigger(Args&&... args);

		private:
			///
			/// Stores the event functions.
			///
			std::vector<Storage> m_event_funcs;
		};

		template<meta::is_class Event, meta::is_class Receiver>
		inline void Dispatcher::subscribe(Receiver& receiver) requires meta::has_on_event_for<Receiver, Event>
		{
			const auto type = meta::DispatcherUID::get<Event>();

			if (type >= m_event_funcs.size())
			{
				m_event_funcs.resize(type + 1);
				m_event_funcs[type].create_storage<Event>();
			}

			m_event_funcs[type].apply_action_to_subscribers<Event, meta::AddAction, Receiver>(receiver);
		}

		template<meta::is_class Event, typename... Args>
		inline void Dispatcher::trigger(Args&&... args)
		{
			const auto type = meta::DispatcherUID::get<Event>();
			if (type >= m_event_funcs.size())
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to trigger event with no subscribers.");
			}
			else
			{
				Event e {std::forward<Args>(args)...};
				m_event_funcs[type].apply_action_to_subscribers<Event, meta::TriggerAction>(e);
			}
		}

	} // namespace events
} // namespace galaxy

#endif