///
/// EventQueue.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_EVENTQUEUE_HPP_
#define GALAXY_EVENTS_EVENTQUEUE_HPP_

#include <variant>
#include <vector>

#include "galaxy/events/KeyDown.hpp"
#include "galaxy/events/KeyRepeat.hpp"
#include "galaxy/events/KeyUp.hpp"
#include "galaxy/events/KeyChar.hpp"
#include "galaxy/events/MouseMoved.hpp"
#include "galaxy/events/MousePressed.hpp"
#include "galaxy/events/MouseReleased.hpp"
#include "galaxy/events/MouseWheel.hpp"
#include "galaxy/events/WindowResized.hpp"

namespace galaxy
{
	namespace events
	{
		///
		/// Event type.
		///
		using Variant = std::variant<std::monostate,
			events::KeyDown,
			events::KeyRepeat,
			events::KeyUp,
			events::KeyChar,
			events::MouseMoved,
			events::MousePressed,
			events::MouseReleased,
			events::MouseWheel,
			events::WindowResized>;

		///
		/// Event queue type.
		///
		using Queue = std::vector<Variant>;
	} // namespace events
} // namespace galaxy

#endif