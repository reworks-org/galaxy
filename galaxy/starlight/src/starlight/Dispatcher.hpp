///
/// Dispatcher.hpp
/// starlight
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_DISPATCHER_HPP_
#define STARLIGHT_DISPATCHER_HPP_

#include "starlight/storage/Storage.hpp"

///
/// Core namespace.
///
namespace starlight
{
	///
	/// This is the main class to dispatch events from.
	///
	class Dispatcher final
	{
	public:
		///
		/// Default constructor.
		///
		Dispatcher() noexcept = default;

		///
		/// Default destructor.
		///
		~Dispatcher() noexcept;

		///
		/// Registers a function to be called on the triggering of an event.
		///
		/// \param callback void function that takes a const Event&.
		///
		template<typename Event>
		void add(const starlight::Callback<Event>& callback) noexcept;

		///
		/// Queues an event to be triggered, does not trigger immediately.
		///
		/// \param args Constructor arguments for event.
		///
		template<typename Event, typename ...Args>
		void queue(Args&&... args) noexcept;

		///
		/// Triggers a single event.
		///
		/// \param event The event to trigger. Calls all callbacks associated with the event.
		///
		template<typename Event>
		void trigger(const Event& event) noexcept;

		///
		/// Triggers all the events in the queue, in order.
		///
		void trigger() noexcept;

	private:
		///
		/// Holds events and their callbacks.
		///
		std::vector<StoragePtr> m_callbacks;

		///
		/// Holds queued events.
		///
		std::vector<QueuedEventPtr> m_queued;
	};
	
	template<typename Event>
	inline void Dispatcher::add(const starlight::Callback<Event>& callback) noexcept
	{
		// Useful to retrieve a compile time unique id.
		const std::size_t typeIndex = UniqueEventID::get<Event>();

		// Ensure leftover references to unique pointer are destroyed.
		{
			// Make sure there is room.
			if (typeIndex >= m_callbacks.size())
			{
				m_callbacks.resize(typeIndex + 1);
			}

			// If null ptr, then no storage for this component exists.
			if (!m_callbacks[typeIndex])
			{
				// Use polymorphism to ensure type erasure.
				// Matches to vector location.
				// This works because the type order is 0..1..2 etc, so there are no blanks in the vector.
				
				m_callbacks[typeIndex] = std::make_unique<starlight::Storage<Event>>();
			}

			// Now convert the storage to the type we want to access.
			auto convertedStorage = static_cast<starlight::Storage<Event>*>(m_callbacks[typeIndex].get());
			convertedStorage->forward(callback);
		}
	}

	template<typename Event, typename ...Args>
	inline void Dispatcher::queue(Args&&... args) noexcept
	{
		m_queued.push_back(std::make_unique<starlight::SpecificEvent<Event>>(std::forward<Args>(args)...));
	}

	template<typename Event>
	inline void Dispatcher::trigger(const Event& event) noexcept
	{
		// Useful to retrieve a compile time unique id.
		const std::size_t typeIndex = UniqueEventID::get<Event>();

		// Matches to vector location and trigger event.
		if (!m_callbacks.empty())
		{
			static_cast<starlight::Storage<Event>*>(m_callbacks[typeIndex].get())->trigger(event);
		}
	}
}

#endif