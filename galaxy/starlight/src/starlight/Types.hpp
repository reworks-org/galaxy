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

#include "protostar/utility/UniqueID.hpp"

///
/// Core namespace.
///
namespace starlight
{
	///
	/// Forward dec.
	///
	class BasicStorage;
	class QueuedEvent;

	///
	/// Shorthand for storage type.
	///
	using StoragePtr = std::unique_ptr<starlight::BasicStorage>;

	///
	/// Shorthand for event type.
	///
	using QueuedEventPtr = std::unique_ptr<starlight::QueuedEvent>;

	///
	/// Predefinition of unique id structure for events.
	///
	using UniqueEventID = protostar::UniqueID<struct EUID>;

	///
	/// Type representing a callback.
	///
	template<typename Event>
	using Callback = std::function<void(const Event&)>;
}

#endif