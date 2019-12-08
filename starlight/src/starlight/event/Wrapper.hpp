///
/// EventWrapper.hpp
/// starlight
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_EVENTWRAPPER_HPP_
#define STARLIGHT_EVENTWRAPPER_HPP_

#include <vector>

#include "starlight/event/Base.hpp"

///
/// Core namespace.
///
namespace starlight
{
	///
	/// Wraps around an Event and Callback(s) assossiated with the event.
	///
	template<typename Event, typename Callback>
	class EventWrapper final : public EventBase
	{
	public:
		///
		/// Constructor.
		///
		EventWrapper() noexcept = default;

		///
		/// Destructor.
		///
		~EventWrapper() override;

		///
		/// Adds the callback to the event wrapper.
		///
		/// \param callback Callback function of type template Callback.
		///
		void add(const Callback& callback);

		///
		/// Triggers the callback function.
		///
		/// \param event Event data to pass to callback. Data must be same type as wrapper.
		///
		void trigger(const std::any& event) override;

	protected:
		///
		/// Stored callbacks.
		///
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
			// This is where any is casted back into appropriate type for the callback.
			callback(std::any_cast<Event>(event));
		}
	}
}

#endif