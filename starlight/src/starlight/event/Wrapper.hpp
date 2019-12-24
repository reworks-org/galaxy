///
/// EventWrapper.hpp
/// starlight
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_EVENTWRAPPER_HPP_
#define STARLIGHT_EVENTWRAPPER_HPP_

#include <vector>
#include <mutex>
#include <future>

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
		/* needs to assert they are not arithmatic...*/
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
		/// Mutex to prevent resources being simultaneously accessed.
		///
		std::mutex m_lock;

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
		if (!m_callbacks.empty())
		{
			// all future results of threads
			std::vector<std::future<void>> futures;

			for (auto& callback : m_callbacks)
			{
				// Launch thread(s) to run event callback on.
				auto result = std::async(std::launch::async, [&]()
					{
						std::lock_guard<std::mutex> lock(m_lock);

						// This is where any is casted back into appropriate type for the callback.
						callback(std::any_cast<Event>(event));
					});

				futures.emplace_back(std::move(result));
			}

			// check to make sure each result has finished. will block main thread but other threads continue running...
			// so it will come out faster.
			// i.e. might be waiting oin thread 1 but thread 2 is already done so it will not blopck when checking thread 2.
			for (auto& f : futures)
			{
				f.get();
			}
		}
	}
}

#endif