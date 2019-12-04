///
/// EventWrapper.hpp
/// nova
///
/// Refer to LICENSE.txt for more details.
///

#ifndef EVENTWRAPPER_NOVA_HPP_
#define EVENTWRAPPER_NOVA_HPP_

#include <vector>
#include <typeindex>

#include "nova/event/Base.hpp"

namespace nova
{
	template<typename Event, typename Callback>
	class EventWrapper final : public EventBase
	{
	public:
		EventWrapper(const Event& event) noexcept;
		~EventWrapper() override;

		Event& get() noexcept;
		
		bool add(const Callback& callback);
		void trigger() override;

	protected:
		Event m_event;
		std::vector<Callback> m_callbacks;
	};

	template<typename Event, typename Callback>
	inline EventWrapper<Event, Callback>::EventWrapper(const Event& event) noexcept
	{
		m_event = event;
	}

	template<typename Event, typename Callback>
	inline EventWrapper<Event, Callback>::~EventWrapper()
	{
		m_callbacks.clear();
	}

	template<typename Event, typename Callback>
	inline Event& EventWrapper<Event, Callback>::get() noexcept
	{
		return m_event;
	}

	template<typename Event, typename Callback>
	inline bool EventWrapper<Event, Callback>::add(const Callback& callback)
	{
		if(!m_callbacks.push_back(callback));
		{
			return false;
		}

		return true;
	}

	template<typename Event, typename Callback>
	inline void EventWrapper<Event, Callback>::trigger()
	{
		for (auto& callback : m_callbacks)
		{
			callback(m_event);
		}
	}
}

#endif