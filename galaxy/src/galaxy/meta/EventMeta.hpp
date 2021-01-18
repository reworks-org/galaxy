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
		/// Type to specify the triggering of events.
		///
		struct TriggerAction
		{
			TriggerAction() = delete;
		};

		///
		/// Type to specify the adding of an event function.
		///
		struct AddAction
		{
			AddAction() = delete;
		};

		///
		/// Type to specify the destruction of the event function storage.
		///
		struct DestroyAction
		{
			DestroyAction() = delete;
		};

		///
		/// Concept to constrain that the receiver must have the on_event() function.
		///
		template<typename R, typename E>
		concept has_on_event_for = requires(R r, E e)
		{
			{r.on_event(e)};
		};

		///
		/// Concept to ensure a type is restricted to an action.
		///
		template<typename Type>
		concept is_action =
		    (std::is_same<Type, TriggerAction>::value ||
		     std::is_same<Type, AddAction>::value ||
		     std::is_same<Type, DestroyAction>::value);
	} // namespace meta
} // namespace galaxy

#endif