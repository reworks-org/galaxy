///
/// Storage.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_DISPATCHER_STORAGE_HPP_
#define GALAXY_EVENTS_DISPATCHER_STORAGE_HPP_

#include <any>
#include <algorithm>
#include <vector>

#include "galaxy/error/Log.hpp"
#include "galaxy/meta/Concepts.hpp"
#include "galaxy/meta/EventMeta.hpp"

namespace galaxy
{
	namespace events
	{
		///
		/// Stores callbacks associated with a particular type.
		///
		class Storage final
		{
		public:
			///
			/// Constructor.
			///
			Storage() noexcept = default;

			///
			/// Move constructor.
			///
			Storage(Storage&&) noexcept;

			///
			/// Move assignment operator.
			///
			Storage& operator=(Storage&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~Storage() noexcept = default;

			///
			/// Create storage.
			///
			template<meta::is_class Event>
			void create_storage();

			///
			/// \brief Specify an action to perform on the event function storage.
			///
			///	Event parameter is the event to store a callback or to access callbacks.
			/// Action is the action to perform on the event function storage.
			///
			/// \param args Can be either the reciever or the event constructor arguments.
			///
			template<meta::is_class Event, meta::EventActions action, typename... Args>
			void apply_action_to_subscribers(Args&... args);

		private:
			///
			/// Copy constructor.
			///
			Storage(const Storage&) = delete;

			///
			/// Copy assignment operator.
			///
			Storage& operator=(const Storage&) = delete;

		private:
			///
			/// Stores event functions utilizing type erasure.
			///
			std::any m_event_functions;
		};

		template<meta::is_class Event>
		inline void Storage::create_storage()
		{
			m_event_functions = std::make_any<std::vector<std::function<void(const Event&)>>>();
		}

		template<meta::is_class Event, meta::EventActions action, typename... Args>
		inline void Storage::apply_action_to_subscribers(Args&... args)
		{
			// Array is only initialized once.
			// So data is preserved as if this was a class member.
			auto& funcs = std::any_cast<std::vector<std::function<void(const Event&)>>&>(m_event_functions);

			// Discard unused branches and only compile used option.
			if constexpr (action == meta::EventActions::TRIGGER)
			{
				// Exec each in parallel.
				std::for_each(funcs.begin(), funcs.end(), [&](auto& func) {
					func(args...);
				});
			}
			else if constexpr (action == meta::EventActions::ADD)
			{
				if constexpr (sizeof...(Args) == 1)
				{
					funcs.emplace_back([&args...](const Event& external_event) {
						(args.on_event(external_event), ...);
					});
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Too many arguments being passed when adding event class.");
				}
			}
			else if constexpr (action == meta::EventActions::ADD_CALLBACK)
			{
				if constexpr (sizeof...(Args) == 1)
				{
					funcs.emplace_back(args...);
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Too many arguments being passed when adding event callback.");
				}
			}
			else if constexpr (action == meta::EventActions::DESTROY)
			{
				funcs.clear();
			}
		}
	} // namespace events
} // namespace galaxy

#endif