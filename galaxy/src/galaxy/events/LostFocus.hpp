///
/// LostFocus.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_LOSTFOCUS_HPP_
#define GALAXY_EVENTS_LOSTFOCUS_HPP_

#include "galaxy/events/Event.hpp"

namespace galaxy
{
	///
	/// \brief POD tag to allow a function to accept a dispatched focus lost event.
	///
	/// Does not have an "handled" member, since all listeners are expected to handle this.
	///
	struct LostFocus final : public Event
	{
	};
} // namespace galaxy

#endif
