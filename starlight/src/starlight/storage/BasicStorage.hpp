///
/// BasicStorage.hpp
/// starlight
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_BASICSTORAGE_HPP_
#define STARLIGHT_BASICSTORAGE_HPP_

///
/// Core namespace.
///
namespace starlight
{
	///
	/// Forward dec.
	///
	class QueuedEvent;

	///
	/// Erases class template type from starlight::Storage.
	///
	class BasicStorage
	{
	public:
		///
		/// Default virtual destructor.
		///
		virtual ~BasicStorage() noexcept = default;

		///
		/// Triggers a basic Queued Event.
		///
		/// \param event Pointer to event to trigger.
		///
		virtual void trigger(QueuedEvent* event) noexcept = 0;

	protected:
		///
		/// Protected default constructor.
		///
		BasicStorage() noexcept = default;
	};
}

#endif