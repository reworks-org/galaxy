///
/// Storage.hpp
/// starlight
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_STORAGE_HPP_
#define STARLIGHT_STORAGE_HPP_

#include <vector>

#include <protostar/system/Concepts.hpp>

#include "starlight/detail/Meta.hpp"

///
/// Core namespace.
///
namespace sl
{
	///
	/// Stores callbacks associated with a particular type.
	///
	struct Storage final
	{
		///
		/// Constructor.
		///
		Storage() noexcept;

		///
		/// Copy constructor.
		///
		Storage(const Storage&) noexcept = default;

		///
		/// Move constructor.
		///
		Storage(Storage&&) noexcept = default;

		///
		/// Copy assignment operator.
		///
		Storage& operator=(const Storage&) noexcept = default;

		///
		/// Move assignment operator.
		///
		Storage& operator=(Storage&&) noexcept = default;

		///
		/// Destructor.
		///
		~Storage() noexcept;

		///
		/// \brief Specify an action to perform on the event function storage.
		///
		///	Event parameter is the event to store a callback or to access callbacks.
		/// Action is the action to perform on the event function storage.
		///
		/// \param args Can be either the reciever or the event constructor arguments.
		///
		template<pr::is_class Event, is_action Action, typename... Args>
		void apply_action_to_subscribers(Args&... args);
	};

	template<pr::is_class Event, is_action Action, typename... Args>
	inline void Storage::apply_action_to_subscribers(Args&... args)
	{
		// Array is only initialized once.
		// So data is preserved as if this was a class member.
		static std::vector<std::function<void(const Event&)>> s_event_functions;

		// Figure out at compile time which action to execute for this function instance.
		constexpr bool is_trigger = std::is_same<Action, sl::TriggerAction>::value;
		constexpr bool is_add     = std::is_same<Action, sl::AddAction>::value;
		constexpr bool is_destroy = std::is_same<Action, sl::DestroyAction>::value;

		// Discard unused branches and only compile used option.
		if constexpr (is_trigger)
		{
			for (auto&& func : s_event_functions)
			{
				func(std::forward<Args>(args)...);
			}
		}
		else if constexpr (is_add)
		{
			s_event_functions.emplace_back([&](const Event& external_event) {
				// This check ensure that this is only compiled if a single receiver is passed...
				constexpr auto length = sizeof...(args);
				if constexpr (length == 1)
				{
					// ...so that this will always expands to just one call.
					(args.on_event(external_event), ...);
				}
			});
		}
		else if constexpr (is_destroy)
		{
			s_event_functions.clear();
		}
	}
} // namespace sl

#endif