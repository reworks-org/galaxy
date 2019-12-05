///
/// Dispatcher.cpp
/// nova
///
/// Refer to LICENSE.txt for more details.
///

#include "Dispatcher.hpp"

namespace nova
{
	void Dispatcher::trigger()
	{
		while (!m_queue.empty())
		{
			m_stored[m_queue.front().m_type]->trigger(m_queue.front().m_event);
			m_queue.pop();
		}
	}
}