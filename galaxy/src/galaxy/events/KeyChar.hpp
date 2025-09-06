///
/// KeyChar.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_KEYCHAR_HPP_
#define GALAXY_EVENTS_KEYCHAR_HPP_

namespace galaxy
{
	///
	/// Contains UTF codepoint.
	///
	struct KeyChar final : public HandleableEvent
	{
		///
		/// UTF codepoint.
		///
		unsigned int m_codepoint = 0u;
	};
} // namespace galaxy

#endif
