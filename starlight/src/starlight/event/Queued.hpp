///
/// Queued.hpp
/// starlight
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUEUED_STARLIGHT_HPP_
#define QUEUED_STARLIGHT_HPP_

#include <any>

namespace starlight
{
	struct QueuedEvent
	{
		QueuedEvent(const std::any& event, const std::size_t type);

		std::any m_event;
		std::size_t m_type;
	};
}

#endif