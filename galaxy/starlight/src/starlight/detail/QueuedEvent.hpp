///
/// QueuedEvent.hpp
/// starlight
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_QUEUEDEVENT_HPP_
#define STARLIGHT_QUEUEDEVENT_HPP_

#include <cstddef>

///
/// Core namespace.
///
namespace starlight
{
	///
	/// Base event type to store, erases type.
	///
	class QueuedEvent
	{
	public:
		///
		/// Virtual default destructor.
		///
		virtual ~QueuedEvent() noexcept = default;

		///
		/// Returns the specific index of this type.
		///
		virtual const std::size_t index() noexcept = 0;

	protected:
		///
		/// Protected default constructor.
		///
		QueuedEvent() noexcept = default;
	};
}

#endif