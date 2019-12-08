///
/// Queued.hpp
/// starlight
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_QUEUED_HPP_
#define STARLIGHT_QUEUED_HPP_

#include <any>

///
/// Core namespace.
///
namespace starlight
{
	///
	/// Stores information about a queued event and erases type.
	///
	struct QueuedEvent
	{
		///
		/// Constructor.
		///
		/// \param event A std::any representing an event to be queued. Allows for the type to be erased.
		/// \param type This is the id of the type retrieved by starlight::UniqueID.
		///
		QueuedEvent(const std::any& event, const std::size_t type);

		///
		/// An event. Its std::any in order to erase type.
		///
		std::any m_event;

		///
		/// The unique id retrieved by starlight::UniqueID to keep track of the type.
		///
		std::size_t m_type;
	};
}

#endif