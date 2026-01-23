///
/// WindowClosed.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_WINDOWCLOSED_HPP_
#define GALAXY_EVENTS_WINDOWCLOSED_HPP_

#include "galaxy/events/Event.hpp"

namespace galaxy
{
	///
	/// Blank "Tag" class/type to signal the window is being closed.
	///
	struct WindowClosed final : public Event
	{
	};
} // namespace galaxy

#endif
