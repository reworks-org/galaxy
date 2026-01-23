///
/// GainedFocus.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_GAINEDFOCUS_HPP_
#define GALAXY_EVENTS_GAINEDFOCUS_HPP_

#include "galaxy/events/Event.hpp"

namespace galaxy
{
	///
	/// POD tag to allow a function to accept a dispatched focus gained event.
	///
	struct GainedFocus final : public Event
	{
	};
} // namespace galaxy

#endif
