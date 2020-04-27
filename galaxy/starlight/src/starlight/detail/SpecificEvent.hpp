///
/// SpecificEvent.hpp
/// starlight
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_SPECIFICEVENT_HPP_
#define STARLIGHT_SPECIFICEVENT_HPP_

#include <utility>

#include "starlight/Types.hpp"
#include "starlight/detail/QueuedEvent.hpp"

///
/// Core namespace.
///
namespace starlight
{
	///
	/// Stores information about a Specific event and erases type.
	///
	template<typename Event, typename ...Args>
	class SpecificEvent final : public starlight::QueuedEvent
	{
	public:
		///
		/// Argument constructor.
		///
		/// \param args Constructor arguments for event.
		///
		SpecificEvent(Args&&... args) noexcept;

		///
		/// Destructor.
		///
		~SpecificEvent() noexcept override = default;

		///
		/// Get specific index type of this event.
		///
		const std::size_t index() noexcept override;

		///
		/// Gets stored event.
		///
		/// \return Reference to event as base type.
		///
		Event& getEvent() noexcept;

	private:
		///
		/// Stored event.
		///
		Event m_event;
	};

	template<typename Event, typename ...Args>
	inline SpecificEvent<Event, Args...>::SpecificEvent(Args&& ...args) noexcept
		:m_event(std::forward<Args>(args)...)
	{
	}

	template<typename Event, typename ...Args>
	inline const std::size_t SpecificEvent<Event, Args...>::index() noexcept
	{
		return UniqueEventID::get<Event>();
	}

	template<typename Event, typename ...Args>
	inline Event& SpecificEvent<Event, Args...>::getEvent() noexcept
	{
		return m_event;
	}
}

#endif