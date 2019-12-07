///
/// Dispatcher.hpp
/// starlight
///
/// Refer to LICENSE.txt for more details.
///

#ifndef DISPATCHER_STARLIGHT_HPP_
#define DISPATCHER_STARLIGHT_HPP_

#include <queue>
#include <memory>
#include <functional>

#include "starlight/event/Queued.hpp"
#include "starlight/event/Wrapper.hpp"
#include "starlight/detail/UniqueID.hpp"

namespace starlight
{
	///
	/// 
	///
	class Dispatcher
	{
		///
		/// Type representing a callback.
		///
		template<typename Event>
		using callback = std::function<void(const Event&)>;

	public:
		///
		/// Adds 
		/// 
		template<typename Event>
		bool add(const callback<Event>& callback);

		///
		/// 
		///
		template<typename Event>
		bool queue(const Event& event);

		///
		/// Triggers single event.
		///
		template<typename Event>
		void trigger(const Event& event);

		///
		/// Triggers all in queue.
		///
		void trigger();

	private:
		std::queue<QueuedEvent> m_queue;
		std::vector<std::unique_ptr<EventBase>> m_stored;
	};
	
	template<typename Event>
	inline bool Dispatcher::add(const callback<Event>& func)
	{
		auto type = EventUniqueID::uid<Event>();
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
		auto type = EventUniqueID::uid<Event>();
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
				m_queue.push({event, type});
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
		auto type = EventUniqueID::uid<Event>();
		m_stored[type]->trigger(event);
	}
}

#endif