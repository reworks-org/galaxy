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

#include <pulsar/Log.hpp>

#include "starlight/detail/Storage.hpp"

///
/// Core namespace.
///
namespace sl
{
	///
	/// Concept to constrain that the receiver must have the on_event() function.
	///
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
		Dispatcher() = default;

		///
		/// Copy constructor.
		///
		Dispatcher(const Dispatcher&) = default;

		///
		/// Move constructor.
		///
		Dispatcher(Dispatcher&&) = default;

		///
		/// Copy assignment operator.
		///
		Dispatcher& operator=(const Dispatcher&) = default;

		///
		/// Move assignment operator.
		///
		Dispatcher& operator=(Dispatcher&&) = default;

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
		template<pr::is_class Event, pr::is_class Receiver>
		void subscribe(Receiver& receiver) requires has_on_event_for<Receiver, Event>;

		///
		/// Triggers a single event.
		///
		/// \param args Arguments to construct event to trigger.
		///
		template<pr::is_class Event, typename... Args>
		void trigger(Args&&... args);

	private:
		///
		/// Stores the event functions.
		///
		std::vector<Storage> m_event_funcs;
	};

	template<pr::is_class Event, pr::is_class Receiver>
	inline void Dispatcher::subscribe(Receiver& receiver) requires has_on_event_for<Receiver, Event>
	{
		const auto type = DispatcherUID::get<Event>();

		if (type >= m_event_funcs.size())
		{
			m_event_funcs.resize(type + 1);
			m_event_funcs[type].create_storage<Event>();
		}

		m_event_funcs[type].apply_action_to_subscribers<Event, sl::AddAction, Receiver>(receiver);
	}

	template<pr::is_class Event, typename... Args>
	inline void Dispatcher::trigger(Args&&... args)
	{
		const auto type = DispatcherUID::get<Event>();
		if (type >= m_event_funcs.size())
		{
			PL_LOG(PL_WARNING, "Attempted to trigger event with no subscribers.");
		}
		else
		{
			Event e {std::forward<Args>(args)...};
			m_event_funcs[type].apply_action_to_subscribers<Event, sl::TriggerAction>(e);
		}
	}
} // namespace sl

#endif