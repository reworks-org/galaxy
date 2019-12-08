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
		// Launch thread to log to.
		auto result = std::async(std::launch::async, [&]()
		{
			std::lock_guard<std::mutex> lock(m_lock);

			while (!m_queue.empty())
			{
				// Executes first in queue then pops, moving on until done.
				m_stored[m_queue.front().m_type]->trigger(m_queue.front().m_event);
				m_queue.pop();
			}
		});
	}
}