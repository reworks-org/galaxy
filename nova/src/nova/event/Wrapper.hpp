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
		EventWrapper() noexcept = default;
		~EventWrapper() override;

		void add(const Callback& callback);
		void trigger(const std::any& event) override;

	protected:
		std::vector<Callback> m_callbacks;
	};

	template<typename Event, typename Callback>
	inline EventWrapper<Event, Callback>::~EventWrapper()
	{
		m_callbacks.clear();
	}

	template<typename Event, typename Callback>
	inline void EventWrapper<Event, Callback>::add(const Callback& callback)
	{
		m_callbacks.push_back(callback);
	}

	template<typename Event, typename Callback>
	inline void EventWrapper<Event, Callback>::trigger(const std::any& event)
	{
		for (auto& callback : m_callbacks)
		{
			callback(std::any_cast<Event>(event));
		}
	}
}

#endif