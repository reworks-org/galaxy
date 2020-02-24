///
/// Dispatcher.cpp
/// starlight
///
/// Refer to LICENSE.txt for more details.
///

#include "Dispatcher.hpp"

///
/// Core namespace.
///
namespace starlight
{
	void Dispatcher::trigger()
	{
		if (!m_queue.empty())
		{
			// all future results of threads
			std::vector<std::future<void>> futures;

			// Launch thread(s) to run event callback on.
			std::for_each(m_queue.begin(), m_queue.end(), [&](const starlight::QueuedEvent& event)
				{
					auto result = std::async(std::launch::async, [&]()
						{
							std::lock_guard<std::mutex> lock(m_lock);
							m_stored[event.m_type]->trigger(event.m_event);
						});

					futures.emplace_back(std::move(result));
				});

			// check to make sure each result has finished. will block main thread but other threads continue running...
			// so it will come out faster.
			// i.e. might be waiting oin thread 1 but thread 2 is already done so it will not blopck when checking thread 2.
			for (auto& f : futures)
			{
				f.get();
			}

			// empty queue.
			m_queue.erase(m_queue.begin(), m_queue.end());
			if (!m_queue.empty())
			{
				// throw exception - should be empty.
				throw std::runtime_error("m_queue should be empty!");
			}
		}
	}
}