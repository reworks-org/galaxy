///
/// Dispatcher.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_DISPATCHER_HPP_
#define GALAXY_EVENTS_DISPATCHER_HPP_

#include <any>
#include <algorithm>
#include <functional>
#include <vector>

#include <robin_hood.h>

#include "galaxy/error/Log.hpp"
#include "galaxy/meta/Concepts.hpp"
#include "galaxy/meta/EventMeta.hpp"

namespace galaxy
{
	namespace events
	{
		///
		/// This is the main class to dispatch events from.
		///
		class Dispatcher final
		{
		public:
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

			///
			/// Constructor.
			///
			Dispatcher() noexcept = default;

			///
			/// Move constructor.
			///
			Dispatcher(Dispatcher&&) noexcept;

			///
			/// Move assignment operator.
			///
			Dispatcher& operator=(Dispatcher&&) noexcept;

			///
			/// Destructor.
			///
			~Dispatcher();

			///
			/// \brief Registers an on_event function from a reciever.
			///
			/// Function will be called on the triggering of an event.
			/// Event is the event to register.
			/// Receiver is the type of object.
			///
			/// \param reciever Object that has an on_event() function that takes a const Event&.
			///
			template<meta::is_class Event, meta::is_class Receiver>
			void subscribe(Receiver& receiver) requires meta::has_on_event_for<Receiver, Event>;

			///
			/// Registers a function callback for an event, without needing a class.
			///
			/// \param func Function to set for event.
			///
			template<meta::is_class Event, typename Lambda>
			void subscribe_callback(Lambda&& func);

			///
			/// Triggers a single event.
			///
			/// \param args Arguments to construct event to trigger.
			///
			template<meta::is_class Event, typename... Args>
			void trigger(Args&&... args);

			///
			/// Clear out data.
			///
			void clear();

		private:
			///
			/// Copy constructor.
			///
			Dispatcher(const Dispatcher&) = delete;

			///
			/// Copy assignment operator.
			///
			Dispatcher& operator=(const Dispatcher&) = delete;

			///
			/// Checks if an event storage needs to be constructed.
			///
			/// \return Returns event type from DispatcherUID.
			///
			template<meta::is_class Event>
			const std::size_t construct_storage();

		private:
			///
			/// Stores the event functions.
			///
			robin_hood::unordered_flat_map<std::size_t, Storage> m_event_funcs;
		};

		template<meta::is_class Event>
		inline void Dispatcher::Storage::create_storage()
		{
			m_event_functions = std::make_any<std::vector<std::function<void(const Event&)>>>();
		}

		template<meta::is_class Event, meta::EventActions action, typename... Args>
		inline void Dispatcher::Storage::apply_action_to_subscribers(Args&... args)
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

		template<meta::is_class Event, meta::is_class Receiver>
		inline void Dispatcher::subscribe(Receiver& receiver) requires meta::has_on_event_for<Receiver, Event>
		{
			const auto type = construct_storage<Event>();
			m_event_funcs[type].apply_action_to_subscribers<Event, meta::EventActions::ADD, Receiver>(receiver);
		}

		template<meta::is_class Event, typename Lambda>
		inline void Dispatcher::subscribe_callback(Lambda&& func)
		{
			const auto type = construct_storage<Event>();
			m_event_funcs[type].apply_action_to_subscribers<Event, meta::EventActions::ADD_CALLBACK>(func);
		}

		template<meta::is_class Event, typename... Args>
		inline void Dispatcher::trigger(Args&&... args)
		{
			const auto type = meta::DispatcherUID::get<Event>();
			if (m_event_funcs.contains(type))
			{
				Event e {std::forward<Args>(args)...};
				m_event_funcs[type].apply_action_to_subscribers<Event, meta::EventActions::TRIGGER>(e);
			}
		}

		template<meta::is_class Event>
		inline const std::size_t Dispatcher::construct_storage()
		{
			const auto type = meta::DispatcherUID::get<Event>();

			if (!m_event_funcs.contains(type))
			{
				m_event_funcs.emplace(type, Storage {});
				m_event_funcs[type].create_storage<Event>();
			}

			return type;
		}

	} // namespace events
} // namespace galaxy

#endif