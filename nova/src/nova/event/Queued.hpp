///
/// Queued.hpp
/// nova
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUEUED_NOVA_HPP_
#define QUEUED_NOVA_HPP_

#include <any>

namespace nova
{
	struct QueuedEvent
	{
		QueuedEvent(const std::any& event, const std::size_t type);

		std::any m_event;
		std::size_t m_type;
	};
}

#endif