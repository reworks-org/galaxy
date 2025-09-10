///
/// KeyInput.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_KEYINPUT_HPP_
#define GALAXY_EVENTS_KEYINPUT_HPP_

#include <string>

#include "galaxy/events/HandleableEvent.hpp"

namespace galaxy
{
	///
	/// Unicode text data from a textinput event.
	///
	struct KeyInput final : public HandleableEvent
	{
		///
		/// Text.
		///
		std::string m_text;
	};
} // namespace galaxy

#endif
