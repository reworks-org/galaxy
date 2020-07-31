///
/// Types.hpp
/// starlight
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_TYPES_HPP_
#define STARLIGHT_TYPES_HPP_

#include <memory>
#include <functional>

#include <protostar/system/UniqueID.hpp>

///
/// Core namespace.
///
namespace sl
{
	///
	/// Forward dec.
	///
	class BasicStorage;
	class QueuedEvent;

	///
	/// Shorthand for storage type.
	///
	using StoragePtr = std::unique_ptr<sl::BasicStorage>;

	///
	/// Shorthand for event type.
	///
	using QueuedEventPtr = std::unique_ptr<sl::QueuedEvent>;

	///
	/// Predefinition of unique id structure for events.
	///
	using DispatcherUID = pr::UniqueID<struct EUID>;

	///
	/// Type representing a callback.
	///
	template<typename Event>
	using Callback = std::function<void(const Event&)>;

	struct TriggerAction
	{
		TriggerAction() = delete;
	};

	struct AddAction
	{
		AddAction() = delete;
	};

	struct DestroyAction
	{
		DestroyAction() = delete;
	};

	template<typename Type>
	concept is_action =
	    (std::is_same<Type, TriggerAction>::value ||
	     std::is_same<Type, AddAction>::value ||
	     std::is_same<Type, DestroyAction>::value);
} // namespace sl

#endif