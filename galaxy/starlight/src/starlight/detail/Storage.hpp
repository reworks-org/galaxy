///
/// Storage.hpp
/// starlight
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_STORAGE_HPP_
#define STARLIGHT_STORAGE_HPP_

#include <any>
#include <vector>

#include <protostar/system/Concepts.hpp>
#include <pulsar/Log.hpp>

#include "starlight/detail/Meta.hpp"

///
/// Core namespace.
///
namespace sl
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
		virtual ~Storage() noexcept;

		///
		/// Create storage.
		///
		template<pr::is_class Event>
		void create_storage();

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

	private:
		///
		/// Stores event functions utilizing type erasure.
		///
		std::any m_event_functions;
	};

	template<pr::is_class Event>
	inline void Storage::create_storage()
	{
		m_event_functions = std::make_any<std::vector<std::function<void(const Event&)>>>();
	}

	template<pr::is_class Event, is_action Action, typename... Args>
	inline void Storage::apply_action_to_subscribers(Args&... args)
	{
		// Array is only initialized once.
		// So data is preserved as if this was a class member.
		auto* funcs = std::any_cast<std::vector<std::function<void(const Event&)>>>(&m_event_functions);

		// Figure out at compile time which action to execute for this function instance.
		constexpr bool is_trigger = std::is_same<Action, sl::TriggerAction>::value;
		constexpr bool is_add     = std::is_same<Action, sl::AddAction>::value;
		constexpr bool is_destroy = std::is_same<Action, sl::DestroyAction>::value;

		// Discard unused branches and only compile used option.
		if constexpr (is_trigger)
		{
			for (auto&& func : *funcs)
			{
				func(args...);
			}
		}
		else if constexpr (is_add)
		{
			if constexpr (sizeof...(Args) == 1)
			{
				funcs->emplace_back([&args...](const Event& external_event) {
					(args.on_event(external_event), ...);
				});
			}
			else
			{
				PL_LOG(PL_FATAL, "Too many arguments being passed when adding function.");
			}
		}
		else if constexpr (is_destroy)
		{
			funcs->clear();
		}
	}
} // namespace sl

#endif