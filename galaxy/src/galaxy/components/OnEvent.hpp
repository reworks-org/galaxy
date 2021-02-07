///
/// OnEvent.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_ONEVENT_HPP_
#define GALAXY_COMPONENTS_ONEVENT_HPP_

#include <functional>

#include <nlohmann/json_fwd.hpp>

#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Allows an entity to recieve an event.
		///
		template<meta::is_class Event>
		class OnEvent final
		{
		public:
			///
			/// Constructor.
			///
			OnEvent() noexcept = default;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			OnEvent(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			OnEvent(OnEvent&&) noexcept;

			///
			/// Move assignment operator.
			///
			OnEvent& operator=(OnEvent&&) noexcept;

			///
			/// Destructor.
			///
			~OnEvent() noexcept = default;

			///
			/// Function called on an event.
			///
			/// \param Reference to the event data of the event that is happening.
			///
			void on_event(const Event& event);

		public:
			///
			/// Function to call on the event.
			///
			std::function<void(const Event&)> m_on_event;

		private:
			///
			/// Copy assignment operator.
			///
			OnEvent& operator=(const OnEvent&) = delete;

			///
			/// Copy constructor.
			///
			OnEvent(const OnEvent&) = delete;
		};

		template<meta::is_class Event>
		inline OnEvent<Event>::OnEvent(const nlohmann::json& json)
		{
		}

		template<meta::is_class Event>
		inline OnEvent<Event>::OnEvent(OnEvent<Event>&& oe) noexcept
		{
			this->m_on_event = std::move(oe.m_on_event);
		}

		template<meta::is_class Event>
		inline OnEvent<Event>& OnEvent<Event>::operator=(OnEvent<Event>&& oe) noexcept
		{
			if (this != &oe)
			{
				this->m_on_event = std::move(oe.m_on_event);
			}

			return *this;
		}

		template<meta::is_class Event>
		inline void OnEvent<Event>::on_event(const Event& event)
		{
			m_on_event(event);
		}
	} // namespace components
} // namespace galaxy

#endif