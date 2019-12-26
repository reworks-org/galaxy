///
/// Base.hpp
/// starlight
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_EVENTBASE_HPP_
#define STARLIGHT_EVENTBASE_HPP_

#include <any>

///
/// Core namespace.
///
namespace starlight
{
	///
	/// Base event type to store, erases type.
	///
	class EventBase
	{
	public:
		///
		/// Virtual destructor.
		///
		virtual ~EventBase() = default;

		///
		/// Triggers the callback for the event.
		///
		/// \param event std::any to erase event type since we only care about data.
		///
		virtual void trigger(const std::any& event) = 0;

	protected:
		///
		/// Protected default destructgor.
		///
		EventBase() = default;
	};
}

#endif