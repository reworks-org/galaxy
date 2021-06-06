///
/// EventMeta.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_META_EVENTMETA_HPP_
#define GALAXY_META_EVENTMETA_HPP_

#include <memory>
#include <functional>

#include "galaxy/meta/UniqueID.hpp"

namespace galaxy
{
	namespace meta
	{
		///
		/// Forward dec.
		///
		class BasicStorage;
		class QueuedEvent;

		///
		/// Shorthand for storage type.
		///
		using StoragePtr = std::unique_ptr<BasicStorage>;

		///
		/// Shorthand for event type.
		///
		using QueuedEventPtr = std::unique_ptr<QueuedEvent>;

		///
		/// Predefinition of unique id structure for events.
		///
		using DispatcherUID = meta::UniqueID<struct EUID>;

		///
		/// Type representing a callback.
		///
		template<typename Event>
		using Callback = std::function<void(const Event&)>;

		///
		/// Actions dispatcher should take on an event.
		///
		enum class EventActions : short
		{
			///
			/// Type to specify the triggering of events.
			///
			TRIGGER,

			///
			/// Type to specify the adding of an event function.
			///
			ADD,

			///
			/// Type to specify the adding of a callback for an event.
			///
			ADD_CALLBACK,

			///
			/// Type to specify the destruction of the event function storage.
			///
			DESTROY
		};

		///
		/// Concept to constrain that the receiver must have the on_event() function.
		///
		template<typename R, typename E>
		concept has_on_event_for = requires(R r, E e)
		{
			{r.on_event(e)};
		};
	} // namespace meta
} // namespace galaxy

#endif