///
/// Queued.cpp
/// starlight
///
/// Refer to LICENSE.txt for more details.
///

#include "Queued.hpp"

namespace starlight
{
	QueuedEvent::QueuedEvent(const std::any& event, const std::size_t type)
		:m_event(event), m_type(type)
	{
	}
}