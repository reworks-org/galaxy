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
	Dispatcher::~Dispatcher() noexcept
	{
		m_queued.clear();
		m_callbacks.clear();
	}

	void Dispatcher::trigger() noexcept
	{
		if (!m_queued.empty())
		{
			for (auto&& event : m_queued)
			{
				// Matches to vector location and trigger event.
				if ((!m_callbacks.empty()) && (event->index() < m_callbacks.size()))
				{
					m_callbacks[event->index()]->trigger(event.get());
				}
			}
		}

		// Reset since queue is processed.
		m_queued.clear();
	}
}