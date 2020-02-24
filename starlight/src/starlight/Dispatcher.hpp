///
/// Dispatcher.hpp
/// starlight
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_DISPATCHER_HPP_
#define STARLIGHT_DISPATCHER_HPP_

#include <deque>
#include <memory>
#include <functional>

#include "starlight/event/Queued.hpp"
#include "starlight/event/Wrapper.hpp"
#include "protostar/utility/UniqueID.hpp"

///
/// Core namespace.
///
namespace starlight
{
	///
	/// Shorthand for event type.
	///
	using EventPtr = std::unique_ptr<EventBase>;

	///
	/// Predefinition of unique id structure for events.
	///
	using EventUniqueID = protostar::UniqueID<struct EUID>;

	///
	/// This is the main class to dispatch events from.
	///
	class Dispatcher
	{
		///
		/// Type representing a callback.
		///
		template<typename Event>
		using callback = std::function<void(const Event&, std::mutex&)>;

	public:
		///
		/// Default constructor.
		///
		Dispatcher() = default;

		///
		/// Default destructor.
		///
		~Dispatcher() = default;

		///
		/// Registers a function to be called on the triggering of an event.
		///
		/// \param callback void function that takes a const Event&.
		///
		/// \return True on success.
		/// 
		template<typename Event>
		bool add(const callback<Event>& callback);

		///
		/// Queues an event to be triggered, does not trigger immediately.
		///
		/// \param event Event to add to the queue.
		///
		/// \return True on success.
		///
		template<typename Event>
		bool queue(const Event& event);

		///
		/// Triggers a single event.
		///
		/// \param event The event to trigger. Calls all callbacks associated with the event.
		///
		template<typename Event>
		void trigger(const Event& event);

		///
		/// Triggers all the events in the queue, in order.
		///
		void trigger();

	private:
		///
		/// Mutex to prevent resources being simultaneously accessed.
		///
		std::mutex m_lock;

		///
		/// Holds queued events.
		///
		std::deque<starlight::QueuedEvent> m_queue;

		///
		/// Stores callbacks and their associated event type. 
		///
		std::vector<EventPtr> m_stored;
	};
	
	template<typename Event>
	inline bool Dispatcher::add(const callback<Event>& func)
	{
		// Useful to retrieve a compile time unique id.
		auto type = EventUniqueID::get<Event>();
		bool result = true;

		if (type >= m_stored.size())
		{
			m_stored.resize(type + 1);
		}

		// Ensure leftover references to unique pointer are destroyed.
		{
			// If null ptr, then no storage for this component exists.
			if (!m_stored[type])
			{
				// Use polymorphism to ensure type erasure.
				// Matches to vector location.
				// This works because the type order is 0..1..2 etc, so there are no blanks in the vector.
				m_stored[type] = std::make_unique<EventWrapper<Event, callback<Event>>>();
			}

			// Now convert the storage to the type we want to access.
			EventWrapper<Event, callback<Event>>* eventWrapper = static_cast<EventWrapper<Event, callback<Event>>*>(m_stored[type].get());
			eventWrapper->add(func);
		}

		return result;
	}

	template<typename Event>
	inline bool Dispatcher::queue(const Event& event)
	{
		// Useful to retrieve a compile time unique id.
		auto type = EventUniqueID::get<Event>();
		bool result = true;

		if (type < m_stored.size())
		{
			// If null ptr, then no storage for this component exists.
			if (!m_stored[type])
			{
				result = false;
			}
			else
			{
				// Utilizes std::any to erase type.
				m_queue.push_front({event, type});
			}
		}
		else
		{
			result = false;
		}

		return result;
	}

	template<typename Event>
	inline void Dispatcher::trigger(const Event& event)
	{
		// Useful to retrieve a compile time unique id.
		auto type = EventUniqueID::get<Event>();

		// Matches to vector location and trigger event.
		m_stored[type]->trigger(event);
	}
}

#endif