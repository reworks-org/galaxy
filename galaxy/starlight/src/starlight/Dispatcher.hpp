///
/// Dispatcher.hpp
/// starlight
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_DISPATCHER_HPP_
#define STARLIGHT_DISPATCHER_HPP_

#include <functional>
#include <vector>

#include "starlight/detail/Storage.hpp"

///
/// Core namespace.
///
namespace sl
{
	template<typename R, typename E>
	concept has_on_event_for = requires(R r, E e)
	{
		{r.on_event(e)};
	};

	///
	/// This is the main class to dispatch events from.
	///
	class Dispatcher final
	{
	public:
		///
		/// Default constructor.
		///
		Dispatcher();

		///
		/// Default destructor.
		///
		~Dispatcher();

		///
		/// Registers a function to be called on the triggering of an event.
		///
		/// \param reciever Object that has a method you pass to func.
		///
		template<pr::is_class Event, pr::is_class Receiver>
		void subscribe(Receiver& receiver) requires has_on_event_for<Receiver, Event>;

		///
		/// Queues an event to be triggered, does not trigger immediately.
		///
		/// \param args Constructor arguments for event.
		///
		template<typename Event, typename... Args>
		void queue(Args&&... args);

		///
		/// Triggers a single event.
		///
		/// \param event The event to trigger. Calls all callbacks associated with the event.
		///
		template<typename Event>
		void trigger(Event& event);

		///
		/// Triggers all the events in the queue, in order.
		///
		void trigger();

	private:
		std::vector<Storage> m_event_funcs;
	};

	template<pr::is_class Event, pr::is_class Receiver>
	inline void Dispatcher::subscribe(Receiver& receiver) requires has_on_event_for<Receiver, Event>
	{
		const auto type = DispatcherUID::get<Event>();

		if (type >= m_event_funcs.size())
		{
			m_event_funcs.resize(type + 1);
		}

		m_event_funcs[type].apply_action_to_subscribers<Event, sl::AddAction, Receiver>(receiver);
	}
} // namespace sl

#endif