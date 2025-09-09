///
/// HandleableEvent.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_HANDLEABLEVENT_HPP_
#define GALAXY_EVENTS_HANDLEABLEVENT_HPP_

namespace galaxy
{
	///
	/// This allows an event to be marked as being "handled" so
	/// events lower in the stack can no longer consume the event.
	///
	struct HandleableEvent
	{
		///
		/// Has this event been handled/consumed/processed already.
		///
		bool m_handled = false;
	};
} // namespace galaxy

#endif
