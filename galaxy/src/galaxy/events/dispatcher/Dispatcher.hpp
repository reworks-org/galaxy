///
/// Dispatcher.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_DISPATCHER_DISPATCHER_HPP_
#define GALAXY_EVENTS_DISPATCHER_DISPATCHER_HPP_

#include <functional>

#include <robin_hood.h>

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
			/// Constructor.
			///
			Dispatcher() noexcept = default;

			///
			/// Move constructor.
			///
			Dispatcher(Dispatcher&&) noexcept;

			///
			/// Move assignment operator.
			///
			Dispatcher& operator=(Dispatcher&&) noexcept;

			///
			/// Destructor.
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

			///
			/// Clear out data.
			///
			void clear();

		private:
			///
			/// Copy constructor.
			///
			Dispatcher(const Dispatcher&) = delete;

			///
			/// Copy assignment operator.
			///
			Dispatcher& operator=(const Dispatcher&) = delete;

		private:
			///
			/// Stores the event functions.
			///
			robin_hood::unordered_flat_map<std::size_t, Storage> m_event_funcs;
		};

		template<meta::is_class Event, meta::is_class Receiver>
		inline void Dispatcher::subscribe(Receiver& receiver) requires meta::has_on_event_for<Receiver, Event>
		{
			const auto type = meta::DispatcherUID::get<Event>();

			if (!m_event_funcs.contains(type))
			{
				m_event_funcs.emplace(type, Storage {});
				m_event_funcs[type].create_storage<Event>();
			}

			m_event_funcs[type].apply_action_to_subscribers<Event, meta::AddAction, Receiver>(receiver);
		}

		template<meta::is_class Event, typename... Args>
		inline void Dispatcher::trigger(Args&&... args)
		{
			const auto type = meta::DispatcherUID::get<Event>();
			if (!m_event_funcs.contains(type))
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