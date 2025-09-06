///
/// MouseWheel.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_MOUSEWHEEL_HPP_
#define GALAXY_EVENTS_MOUSEWHEEL_HPP_

namespace galaxy
{
	///
	/// Contains mouse wheel movement data.
	///
	struct MouseWheel final : public HandleableEvent
	{
		///
		/// -1 or 1.
		///
		double m_xoff = 0.0;

		///
		/// -1 or 1.
		///
		double m_yoff = 0.0;
	};
} // namespace galaxy

#endif
