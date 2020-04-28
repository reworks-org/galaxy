///
/// Storage.hpp
/// starlight
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_STORAGE_HPP_
#define STARLIGHT_STORAGE_HPP_

#include "starlight/storage/BasicStorage.hpp"
#include "starlight/detail/SpecificEvent.hpp"

///
/// Core namespace.
///
namespace starlight
{
	///
	/// Stores callbacks associated with a particular type.
	///
	template<typename Event>
	class Storage final : public starlight::BasicStorage
	{
	public:
		///
		/// Default constructor.
		///
		Storage() noexcept = default;

		///
		/// Override destructor.
		///
		~Storage() noexcept override;

		///
		/// Forwards callback to construct in place in std::vector.
		///
		/// \param callback Callback to move into std::vector.
		///
		void forward(const Callback<Event>& callback) noexcept;

		///
		/// Trigger all callbacks in order, asynchronously.
		///
		/// \param event The event all callbacks should recieve.
		///
		void trigger(const Event& event) noexcept;

		///
		/// Triggers a basic Queued Event.
		///
		/// \param event Pointer to event to trigger.
		///
		void trigger(QueuedEvent* event) noexcept override;

	private:
		///
		/// Callbacks stored here. index = event unique id.
		///
		std::vector<Callback<Event>> m_callbacks;
	};

	template<typename Event>
	inline Storage<Event>::~Storage() noexcept
	{
		m_callbacks.clear();
	}

	template<typename Event>
	inline void Storage<Event>::forward(const Callback<Event>& callback) noexcept
	{
		m_callbacks.push_back(std::move(callback));
	}

	template<typename Event>
	inline void Storage<Event>::trigger(const Event& event) noexcept
	{
		if (!m_callbacks.empty())
		{
			for (auto&& call : m_callbacks)
			{
				// Launch thread(s) to run event callback on.
				call(event);
			}
		}
	}

	template<typename Event>
	inline void Storage<Event>::trigger(QueuedEvent* event) noexcept
	{
		starlight::SpecificEvent<Event>* specificEvent = static_cast<starlight::SpecificEvent<Event>*>(event);
		trigger(specificEvent->getEvent());
	}
}

#endif