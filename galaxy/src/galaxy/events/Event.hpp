///
/// Event.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_EVENT_HPP_
#define GALAXY_EVENTS_EVENT_HPP_

namespace galaxy
{
	///
	/// Base type of all events.
	///
	class Event
	{
	public:
		///
		/// Constructor.
		///
		Event() noexcept;

		///
		/// Move constructor.
		///
		Event(Event&&) noexcept;

		///
		/// Move assignment operator.
		///
		Event& operator=(Event&&) noexcept;

		///
		/// Copy constructor.
		///
		Event(const Event&) noexcept;

		///
		/// Copy assignment operator.
		///
		Event& operator=(const Event&) noexcept;

		///
		/// Destructor.
		///
		virtual ~Event() noexcept;

		///
		/// Mark this event as consumed.
		///
		void consume() noexcept;

		///
		/// Is this event already consumed?
		///
		/// \return True if this event has been handled already.
		///
		[[nodiscard]]
		bool consumed() const noexcept;

	protected:
		///
		/// Keeps track if event has been used already.
		///
		bool m_handled;
	};
} // namespace galaxy

#endif
