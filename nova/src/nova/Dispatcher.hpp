///
/// Dispatcher.hpp
/// nova
///
/// Refer to LICENSE.txt for more details.
///

#ifndef DISPATCHER_NOVA_HPP_
#define DISPATCHER_NOVA_HPP_

#include <queue>
#include <memory>
#include <functional>

#include "nova/event/Wrapper.hpp"
#include "nova/detail/UniqueID.hpp"

namespace nova
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
		/// no guarantee on ordering of calls.
		/// 
		template<typename Event>
		bool add(const callback<Event>& callback);

		///
		/// add and store
		///
		template<typename Event>
		bool queue(const Event& event);

		///
		/// call
		///
		template<typename Event>
		void triggerQueue();

		///
		/// add and call
		///
		template<typename Event>
		void trigger(const Event& event);

	private:
		std::queue<std::unique_ptr<EventBase>> m_queued;
		std::vector<std::unique_ptr<EventBase>> m_stored;
	};
	
	template<typename Event>
	inline bool Dispatcher::add(const callback<Event>& callback)
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
				m_stored[type] = std::make_unique<EventWrapper<Event, decltype(callback<Event>)>>();
			}

			// Now convert the storage to the type we want to access.
			EventWrapper<Event>* eventWrapper = static_cast<EventWrapper<Event>*>(m_stored[type].get());
			result = eventWrapper->add<Event, decltype(callback<Event>)>(callback);
		}

		return result;
	}

	template<typename Event>
	inline bool Dispatcher::queue(const Event& event)
	{
		m_queued.push(std::make_unique<EventWrapper<Event, decltype(callback<Event>)>>(event));
	}

	template<typename Event>
	inline void Dispatcher::triggerQueue()
	{
		while (!m_queued.empty())
		{
			m_queued.front()->trigger();
			m_queued.pop();
		}
	}

	template<typename Event>
	inline void Dispatcher::trigger(const Event& event)
	{
		auto type = EventUniqueID::uid<Event>();
		m_stored[type]->trigger();
	}
}

#endif